#include "Midea/MideaDevice.hpp"
#include "Helpers/Logger.hpp"

namespace dongle {
namespace midea {

using helpers::Timer;

static const char *const TAG = "MideaDevice";

MideaDevice::MideaDevice()
    : m_wifiNotifier(120000,
                     [this](const helpers::WifiNotifierBase &) { m_sendNetworkNotify(MSG_NOTIFY_NETWORK_STATUS); }),
      m_periodTimer(std::make_unique<Timer>(m_period, false, [this](Timer *) { m_isBusy = false; })),
      m_responseTimer(std::make_unique<Timer>(m_timeout, false, [this](Timer *) {
        LOG_D(TAG, "Response timeout.");

        if (!--m_remainAttempts) {
          LOG_W(TAG, "Request failed. It looks like unsupported options are enabled. Check your YAML configuration.");

          if (m_request->onError != nullptr)
            m_request->onError();

          m_destroyRequest();
          return;
        }

        LOG_D(TAG, "Sending request again. %d attempts left.", m_remainAttempts);

        m_sendRequest(m_request);
        m_resetTimeout();
      })) {}

ResponseStatus MideaDevice::Request::callHandler(const Message &s) {
  if (!s.hasType(this->requestType))
    return ResponseStatus::RESPONSE_WRONG;

  if (this->onData == nullptr)
    return RESPONSE_OK;

  return this->onData(s);
}

void MideaDevice::setup() { m_setup(); }

void MideaDevice::loop() {
  // Timers task
  m_periodTimer->run();
  m_responseTimer->run();
  m_wifiNotifier.run();

  // Loop for appliances
  m_loop();

  // Message receiving
  while (m_frameio.read()) {
    m_handler(m_frameio);
    m_frameio.clear();
  }

  if (m_isBusy || m_isWaitForResponse())
    return;

  if (m_queue.empty()) {
    m_onIdle();
    return;
  }

  m_request = m_queue.front();
  m_queue.pop_front();

  LOG_D(TAG, "Getting and sending a request from the queue.");

  m_sendRequest(m_request);

  if (m_request->onData != nullptr) {
    m_resetAttempts();
    m_resetTimeout();

  } else {
    m_destroyRequest();
  }
}

void MideaDevice::m_handler(const Message &s) {
  if (m_isWaitForResponse()) {
    auto result = m_request->callHandler(s);

    if (result != RESPONSE_WRONG) {
      if (result == RESPONSE_OK) {
        if (m_request->onSuccess != nullptr)
          m_request->onSuccess();

        m_destroyRequest();

      } else {
        m_resetAttempts();
        m_resetTimeout();
      }

      return;
    }
  }

  // ignoring responses on network notifies
  if (s.hasType(MSG_NOTIFY_NETWORK_STATUS))
    return;

  /* HANDLE REQUESTS */
  if (s.hasType(MSG_GET_NETWORK_STATUS)) {
    m_sendNetworkNotify(MSG_GET_NETWORK_STATUS);
    return;
  }

  m_onRequest(s);
}

void MideaDevice::m_sendNetworkNotify(MessageTypeID frameType) {
  switch (frameType) {
    case MSG_NOTIFY_NETWORK_STATUS:
      LOG_D(TAG, "Enqueuing a DEVICE_NETWORK(0x0D) notification.");
      // m_queueRequest(frameType, MessageData::networkNotify(m_wifiNotifier), nullptr);
      break;
    case MSG_GET_NETWORK_STATUS:
      LOG_D(TAG, "Answer to NETWORK_QUERY(0x63) request.");
      // m_sendFrame(frameType, MessageData::networkNotify(m_wifiNotifier));
      break;
    default:
      break;
  }
}

void MideaDevice::m_resetTimeout() { m_responseTimer->start(m_timeout); }

void MideaDevice::m_destroyRequest() {
  LOG_D(TAG, "Destroying the request.");
  m_responseTimer->stop();
  delete m_request;
  m_request = nullptr;
}

void MideaDevice::m_sendFrame(MessageTypeID type, const MessageData &s) {
  m_frameio.write(type, s);
  m_isBusy = true;
  m_periodTimer->start(m_period);
}

void MideaDevice::m_queueRequest(MessageTypeID type, MessageData &&data, ResponseHandler onData, Handler onSucess,
                                 Handler onError) {
  LOG_D(TAG, "Enqueuing the request.");
  m_queue.push_back(new Request{std::move(data), std::move(onData), std::move(onSucess), std::move(onError), type});
}

void MideaDevice::m_queueRequestPriority(MessageTypeID type, MessageData &&data, ResponseHandler onData,
                                         Handler onSucess, Handler onError) {
  LOG_D(TAG, "Priority request queuing.");
  m_queue.push_front(new Request{std::move(data), std::move(onData), std::move(onSucess), std::move(onError), type});
}

void MideaDevice::setBeeper(bool value) {
  LOG_D(TAG, "Turning %s beeper feedback.", value ? "ON" : "OFF");
  m_beeper = value;
}

}  // namespace midea
}  // namespace dongle
