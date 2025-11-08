#pragma once

#include <cstdint>

namespace dongle {
namespace midea {
namespace ac {

struct DeviceStatusC1 {
  /* Byte #0-15 */

  uint8_t id;
  uint8_t unused[15];

  /* Byte #16,17,18 */

  uint8_t bcdPower[3];
};

}  // namespace ac
}  // namespace midea
}  // namespace dongle
