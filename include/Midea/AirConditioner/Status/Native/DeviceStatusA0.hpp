#pragma once

#include "Midea/AirConditioner/Status/Types.hpp"
#include "Midea/AirConditioner/Status/DeviceTimers.hpp"

namespace dongle {
namespace midea {
namespace ac {

struct DeviceStatusA0 {
  /* Byte #0 */

  uint8_t id;

  /* Byte #1 */

  /// Power status flag: `ON/OFF`.
  bool power : 1;
  /// Target temperature - 12°C.
  uint8_t newTemp : 5;
  /// Target temperature `+0.5°C` flag.
  bool dotTemp : 1;
  /// Unknown flag.
  bool errMark : 1;

  /* Byte #2 */

  uint8_t : 5;
  /// Operation mode.
  OperationMode mode : 3;

  /* Byte #3 */

  /// Fan speed. Unit: percents.
  uint8_t fanSpeed : 7;
  /// Skip unused.
  bool : 1;

  /* Bytes #4,5,6 */
  DeviceTimers timers;

  /* Byte #7 */

  /// Horizontal swing.
  uint8_t leftRightFan : 2;
  /// Vertical swing.
  uint8_t updownFan : 2;
  /// Skip unused.
  uint8_t : 0;

  /* Byte #8 */

  /// Unknown value.
  uint8_t cosySleep : 2;
  /// Skip unused.
  bool : 1;
  /// Unknown flag.
  bool save : 1;
  /// Unknown flag.
  bool lowFreqFan : 1;
  /// `TURBO` preset flag.
  bool turbo1 : 1;
  /// Skip unused.
  bool : 1;
  /// Unknown flag.
  bool feelOwn : 1;

  /* Byte #9 */

  /// Skip unused.
  bool : 1;
  /// Unknown flag.
  bool exchangeAir : 1;
  /// Unknown flag.
  bool dryClean : 1;
  /// Electric auxiliary heater flag.
  bool ptcAssis : 1;
  /// ECO mode
  bool eco : 1;
  /// Unknown flag.
  bool cleanUp : 1;
  /// Skip unused.
  bool : 1;
  /// Display unit is `Fahrenheits`.
  bool tempUnit : 1;

  /* Byte #10 */

  /// `SLEEP` preset flag.
  bool sleepFunc : 1;
  /// `TURBO` preset flag.
  bool turbo2 : 1;
  /// Skip unused.
  bool : 1;
  /// Unknown flag.
  bool catchCold : 1;
  /// Unknown flag.
  bool nightLight : 1;
  /// Unknown flag.
  bool peakElec : 1;
  /// Unknown flag.
  bool naturalFan : 1;
  /// Skip unused.
  bool : 1;

  /* Byte #11 */

  /// Unknown value.
  uint8_t pwmMode : 4;
  /// `LED Light` value.
  uint8_t light : 3;
  /// Skip unused.
  uint8_t : 1;

  /* Byte #12 */

  /// Unknown temperature -12°C. Unit : 0.5°C.
  uint8_t setExpand : 6;
  /// Unknown flag.
  bool doubleTemp : 1;
  /// `FROST PROTECTION` preset (8 Degrees Heating).
  bool eightHot : 1;

  /* Byte #13 */

  /// Humidity setpoint in `SMART DRY` mode. Fan speed must be `AUTO`.
  uint8_t humidity : 7;
  /// Skip unused.
  uint8_t : 1;

  /* Byte #14 */

  uint8_t : 3;
  /// `SILKY COOL` preset.
  bool hasNoWindFeel : 1;
  /// Skip unused.
  uint8_t : 4;
};

}  // namespace ac
}  // namespace midea
}  // namespace dongle
