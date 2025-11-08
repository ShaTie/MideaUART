#pragma once

#include <cstdint>

namespace dongle {
namespace midea {
namespace ac {

struct DeviceStatusA1 {
  /* Byte #0-12 */

  uint8_t id;
  uint8_t unused[12];

  /* Byte #13,14 */

  uint8_t inTemp, outTemp;

  /* Byte #15,16 */

  /// Skip unused.
  uint8_t : 8;
  /// Skip unused.
  uint8_t : 8;

  /* Byte #17 */

  /// Humidity setpoint in `SMART DRY` mode. Fan speed must be `AUTO`.
  uint8_t humidity : 7;
  /// Skip unused.
  uint8_t : 1;
};

}  // namespace ac
}  // namespace midea
}  // namespace dongle
