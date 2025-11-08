#include "Dongle/Message/Message.hpp"

namespace dongle {

class Dongle;

class ITransparentDataHandler {
 public:
  /**
   * @brief Dongle is ready to send new message.
   */
  virtual void onDongleIdle(Dongle *dongle) = 0;

  /**
   * @brief Dongle received transparent message.
   *
   * @param data Message data.
   */
  virtual void onDongleData(Dongle *dongle, MessageData data) = 0;
};

}  // namespace dongle
