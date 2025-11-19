#pragma once

#include <deque>
#include <memory>

#include "Dongle/Message/MessageReaderWriter.hpp"
#include "Helpers/Logger.hpp"
#include "Helpers/Timer.hpp"
#include "Helpers/WifiNotifier.hpp"

namespace midea {

/**
 * @brief Midea appliance types.
 *
 */
enum ApplianceID : uint8_t {
  /**
   * @brief Dehumidifier.
   */
  APP_DEHUMIDIFIER = 0xA1,

  /**
   * @brief Air conditioner.
   */
  APP_AIR_CONDITIONER = 0xAC,

  /**
   * @brief Humidifier.
   */
  APP_HUMIDIFIER = 0xFD,

  /**
   * @brief Broadcast.
   */
  APP_BROADCAST = 0xFF,
};

using helpers::Timer;
using helpers::WifiNotifier;

enum AutoconfStatus : uint8_t {
  AUTOCONF_DISABLED,
  AUTOCONF_PROGRESS,
  AUTOCONF_DONE,
  AUTOCONF_ERROR,
};

enum ResponseStatus : uint8_t {
  RESPONSE_OK,
  RESPONSE_PARTIAL,
  RESPONSE_WRONG,
};

using Handler = std::function<void()>;
using ResponseHandler = std::function<ResponseStatus(const Message &)>;
using OnStateCallback = std::function<void()>;

class MideaDevice {
 public:
  MideaDevice();
  /// Setup
  void setup();
  /// Loop
  void loop();

  /* ############################## */
  /* ### COMMUNICATION SETTINGS ### */
  /* ############################## */

  /// Set serial stream.
  void setTransport(ITransport *io) { m_frameio.setTransport(io); }

  /// Set minimal period between requests.
  void setPeriod(uint32_t period) { m_period = period; }

  uint32_t getPeriod() const { return m_period; }

  /// Set waiting response timeout.
  void setTimeout(uint32_t timeout) { m_timeout = timeout; }

  uint32_t getTimeout() const { return m_timeout; }

  /// Set number of request attempts.
  void setNumAttempts(uint8_t numAttempts) { m_numAttempts = numAttempts; }

  uint8_t getNumAttempts() const { return m_numAttempts; }

  /// Set beeper feedback.
  void setBeeper(bool value);

  /// Add listener for appliance state.
  void addOnStateCallback(OnStateCallback cb) { m_stateCallbacks.push_back(cb); }

  void sendUpdate() {
    for (auto &cb : m_stateCallbacks)
      cb();
  }

  AutoconfStatus getAutoconfStatus() const { return m_autoconfStatus; }

  void setAutoconf(bool state) { m_autoconfStatus = state ? AUTOCONF_PROGRESS : AUTOCONF_DISABLED; }

  static void setLogger(helpers::LoggerFn logger) { helpers::Logger::set(logger); }

 protected:
  /// Callbacks of state subscribers.
  std::vector<OnStateCallback> m_stateCallbacks;

  /// Status of autoconfig process.
  AutoconfStatus m_autoconfStatus{};

  // Beeper feedback flag.
  bool m_beeper{};

  void m_queueRequest(MessageTypeID type, MessageData &&data, ResponseHandler onData, Handler onSucess = nullptr,
                      Handler onError = nullptr);

  void m_queueRequestPriority(MessageTypeID type, MessageData &&data, ResponseHandler onData = nullptr,
                              Handler onSucess = nullptr, Handler onError = nullptr);

  void m_sendFrame(MessageTypeID type, const MessageData &data);

  /// Setup for appliances.
  virtual void m_setup() {}

  /// Loop for appliances.
  virtual void m_loop() {}

  /// Calling then ready for request.
  virtual void m_onIdle() {}

  /// Calling on receiving request.
  virtual void m_onRequest(const Message &message) {}

 private:
  struct Request {
    MessageData request;
    ResponseHandler onData;
    Handler onSuccess;
    Handler onError;
    MessageTypeID requestType;
    ResponseStatus callHandler(const Message &data);
  };

  void m_sendNetworkNotify(MessageTypeID typeID);

  void m_handler(const Message &s);

  bool m_isWaitForResponse() const { return m_request != nullptr; }

  void m_resetAttempts() { m_remainAttempts = m_numAttempts; }

  void m_destroyRequest();

  void m_resetTimeout();

  void m_sendRequest(Request *req) { m_sendFrame(req->requestType, req->request); }

  /// Message receiver with dynamic buffer.
  MessageReaderWriter m_frameio{64};

  /// Network status timer.
  WifiNotifier m_wifiNotifier;

  std::unique_ptr<Timer> m_periodTimer;
  std::unique_ptr<Timer> m_responseTimer;

  /// Requests queue.
  std::deque<Request *> m_queue;

  /// Current request.
  Request *m_request{nullptr};

  // Remaining request attempts
  uint8_t m_remainAttempts{};

  /// Period flag
  bool m_isBusy{};

  /* ############################## */
  /* ### COMMUNICATION SETTINGS ### */
  /* ############################## */

  /// Minimal period between requests
  uint32_t m_period{1000};

  /// Waiting response timeout
  uint32_t m_timeout{2000};

  /// Number of request attempts
  uint8_t m_numAttempts{3};

  enum {
    STATE_INIT,
    STATE_,
  };
};

}  // namespace midea
