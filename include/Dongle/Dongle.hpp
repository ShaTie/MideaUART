#include "Dongle/Message/Message.hpp"
#include "Helpers/Timer.hpp"

namespace dongle {

class ITransparentDataHandler;
class ITransport;

using helpers::Timer;

enum MessageMode : uint8_t {
  MODE_UNKNOWN,
  MODE_INTERFRAME,
  MODE_STREAM,
};

class Dongle {
 public:
  void setTransport(ITransport *x) { m_transport = x; }
  void setTransparentDataHandler(ITransparentDataHandler *x) { m_handler = x; }
  ApplianceID applianceID() const { return m_applianceID; }
  uint8_t protocolID() const { return m_protocolID; }
  void task();

 protected:
  ITransport *m_transport;
  ITransparentDataHandler *m_handler;
  Message m_message;
  Timer m_timer;
  ApplianceID m_applianceID;
  uint8_t m_protocolID;

  enum : uint8_t {
    STATE_INIT,
    STATE_BUSY,
    STATE_IDLE,
  } m_state;
};

}  // namespace dongle
