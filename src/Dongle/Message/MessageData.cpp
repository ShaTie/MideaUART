#include "Dongle/Message/MessageData.hpp"
#include "Helpers/WifiNotifier.hpp"

namespace dongle {

std::array<uint8_t, 20> MessageData::networkNotify(const helpers::WifiNotifierBase &x) {
  return {
      1,                        // 0 : RF, 1 : WiFi
      1,                        // 1 : client, 2 : config, 3 : AP
      uint8_t(x.level()),       // WiFi signal strength (0-4)
      uint8_t(x.ip() >> 24),    // IP[A]
      uint8_t(x.ip() >> 16),    // IP[B]
      uint8_t(x.ip() >> 8),     // IP[C]
      uint8_t(x.ip() >> 0),     // IP[D]
      255,                      // 255 : RF not supported
      uint8_t(!x.connected()),  // 0 : connected, 1 : not connected
  };
}

}  // namespace dongle
