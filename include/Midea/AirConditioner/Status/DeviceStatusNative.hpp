#pragma once

#include <type_traits>

#include "Midea/AirConditioner/Status/Types.hpp"
#include "Midea/AirConditioner/Status/DeviceTimers.hpp"

namespace midea {
namespace ac {

struct StatusBase {
  uint8_t id;
};

template<typename T>
concept NativeStatusConcept = std::is_base_of_v<StatusBase, T>;

struct StatusA0 : public StatusBase {
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

struct StatusA1 : public StatusBase {
  /* Byte #1-12 */

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

/**
 * @brief Device state `0xC0` message body. Size: 22 bytes.
 */
struct StatusC0 : public StatusBase {
  // Byte #1

  /// Power status flag: `ON/OFF`.
  bool power : 1;
  /// Skip unused.
  bool : 1;
  /// Unknown flag.
  bool imodeResume : 1;
  /// Skip unused.
  bool : 1;
  /// Unknown flag.
  bool timerMode : 1;
  /// Unknown flag.
  bool test2 : 1;
  /// Skip unused.
  bool : 1;
  /// Unknown flag.
  bool errMark : 1;

  // Byte #2

  /// Target temperature - 16°C. Deprecated.
  uint8_t oldTemp : 4;
  /// Target temperature `+0.5°C` flag.
  bool dotTemp : 1;
  /// Operation mode.
  OperationMode mode : 3;

  // Byte #3

  /// Fan speed. Unit: percents.
  uint8_t fanSpeed : 7;
  /// Skip unused.
  bool : 1;

  // Bytes #4,5,6
  DeviceTimers timers;

  // Byte #7

  /// Horizontal swing.
  uint8_t leftRightFan : 2;
  /// Vertical swing.
  uint8_t updownFan : 2;
  /// Skip unused.
  uint8_t : 0;

  // Byte #8

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

  // Byte #9

  /// Unknown flag.
  bool childSleepMode : 1;
  /// Unknown flag.
  bool naturalFan : 1;
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
  /// Unknown flag.
  bool selfFeelOwn : 1;

  // Byte #10

  /// `SLEEP` preset flag.
  bool sleepFunc : 1;
  /// `TURBO` preset flag.
  bool turbo2 : 1;
  /// Display unit is `Fahrenheits`.
  bool tempUnit : 1;
  /// Unknown flag.
  bool exchangeAir : 1;
  /// Unknown flag.
  bool nightLight : 1;
  /// Unknown flag.
  bool catchCold : 1;
  /// Unknown flag.
  bool peakElec : 1;
  /// Unknown flag.
  bool coolFan : 1;

  // Bytes #11,12

  uint8_t inTemp, outTemp;

  // Byte #13

  /// Target temperature - 12°C.
  uint8_t newTemp : 5;
  /// `Air Filter Maintenance` flag.
  bool dusFull : 1;
  /// Skip unused.
  uint8_t : 2;

  // Byte #14

  /// Unknown value.
  uint8_t pwmMode : 4;
  /// `LED Light` value.
  uint8_t light : 3;
  /// Skip unused.
  uint8_t : 1;

  // Byte #15

  uint8_t inTempDec : 4;
  uint8_t outTempDec : 4;

  // Byte #16

  /// Error code. Known: `0x26` - `Water Full`.
  uint8_t errInfo;

  // Bytes #17,18

  /// Skip unused.
  uint8_t : 8;
  /// Skip unused.
  uint8_t : 8;

  // Byte #19

  /// Humidity setpoint in `SMART DRY` mode. Fan speed must be `AUTO`.
  uint8_t humidity : 7;
  /// Skip unused.
  uint8_t : 1;

  // Byte #20

  /// Skip unused.
  uint8_t : 8;

  // Byte #21

  /// Unknown temperature -12°C. Unit : 0.5°C.
  uint8_t setExpand : 6;
  /// Unknown flag.
  bool doubleTemp : 1;
  /// `FROST PROTECTION` preset (8 Degrees Heating).
  bool eightHot : 1;

  // Byte #22

  uint8_t : 3;
  /// `SILKY COOL` preset capability.
  bool hasNoWindFeel : 1;
  /// Skip unused.
  uint8_t : 4;
};

struct StatusC1 : public StatusBase {
  /* Byte #1-15 */
  uint8_t unused[15];
  /* Byte #16,17,18 */
  uint8_t bcdPower[3];
};

}  // namespace ac
}  // namespace midea
