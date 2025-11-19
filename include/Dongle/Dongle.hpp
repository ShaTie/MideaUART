#pragma once

#include <list>
#include <functional>

#include "Dongle/Message/Message.hpp"
#include "Dongle/IEventHandler.hpp"
#include "Helpers/Timer.hpp"

namespace dongle {

class ITransparentDataHandler;
class ITransport;

enum MessageMode : uint8_t {
  MODE_UNKNOWN,
  MODE_INTERFRAME,
  MODE_STREAM,
};

struct MessageTX;

using MessageCallback = std::function<void(const MessageTX &, const Message &)>;

struct MessageTX {
  // message
  // callback
  // send timestamp
};

class Dongle {
 public:
  void setTransport(ITransport *x) { m_transport = x; }
  void setTransparentDataHandler(ITransparentDataHandler *x) { m_handler = x; }

  /**
   * @brief Gets appliance ID.
   * @return Appliance ID.
   */
  uint8_t applianceID() const { return m_applianceID; }

  /**
   * @brief Gets appliance protocol ID.
   * @return Appliance protocol ID.
   */
  uint8_t protocolID() const { return m_protocolID; }

  void task() {}

  /**
   * @brief Task for transmitting messages.
   */
  void m_txTask();

  /**
   * @brief Task for receiving messages.
   */
  void m_rxTask();

 protected:
  ITransport *m_transport;
  ITransparentDataHandler *m_handler;
  Message m_message;
  std::list<MessageTX> m_txQueue;

  /**
   * @brief
   * @return
   */
  bool m_txDelay();

  /**
   * @brief Main timer.
   *
   * 1. Initial delay for 3000 ms.
   * 2. Awaiting answer for 1000 ms.
   * 3. Delay after answer for 50 ms before sending next request.
   */
  helpers::Timer m_timer;

  /// ID generator.
  helpers::IDGenerator m_id{};

  /// Appliance ID. Default : `255` as broadcast.
  uint8_t m_applianceID{255};

  /// Protocol ID. Default : `0`.
  uint8_t m_protocolID{0};

  enum : uint8_t {
    STATE_INIT,
    STATE_BUSY,
    STATE_IDLE,
  } m_state;
};

}  // namespace dongle
