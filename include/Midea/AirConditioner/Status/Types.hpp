#pragma once

#include <cstdint>

namespace midea {
namespace ac {

/**
 * @brief All operation modes of air conditioner.
 */
enum OperationMode : uint8_t {
  /// Unknown mode. Not updated or not supported.
  MODE_UNKNOWN,
  /// `AUTO` operation mode. Temperature range from `AUTO`.
  MODE_AUTO,
  /// `COOL` operation mode. Temperature range from `COOL`.
  MODE_COOL,
  /// `DRY` operation mode. Temperature range from `COOL`.
  MODE_DRY,
  /// `HEAT` operation mode. Temperature range from `HEAT`.
  MODE_HEAT,
  /// `FAN ONLY` operation mode. Temperature range from `COOL`.
  MODE_FAN,
  /// `DRY SMART` operation mode. Temperature range from `COOL` (in .humidity must be setpoint).
  MODE_DRY_CUSTOM,
};

/**
 * @brief All operation modes of `BREEZELESS` feature.
 */
enum BreezelessMode : uint8_t {
  /// Unknown mode. Not updated or not supported.
  BREEZELESS_UNKNOWN,
  /// `BREEZELESS` preset `OFF` mode.
  BREEZELESS_OFF,
  /// `BREEZELESS` preset `AWAY` mode.
  BREEZELESS_AWAY,
  /// `BREEZELESS` preset `MILD` mode.
  BREEZELESS_MILD,
  /// `BREEZELESS` preset `LESS` mode.
  BREEZELESS_LESS,
};

/**
 * @brief All supported air flow directions.
 */
enum AirFlowDirection : uint8_t {
  /// Unknown direction. Not updated or not supported.
  AIR_DIRECTION_UNKNOWN,
  /// 1st air flow direction.
  AIR_DIRECTION_1,
  /// 2nd air flow direction.
  AIR_DIRECTION_2 = 25,
  /// 3rd air flow direction.
  AIR_DIRECTION_3 = 50,
  /// 4th air flow direction.
  AIR_DIRECTION_4 = 75,
  /// 5th air flow direction.
  AIR_DIRECTION_5 = 100,
};

/**
 * @brief Fan speeds presets enumeration.
 *
 * 1. The fan speed unit is `%`. For `AUTO` used value 102.
 *
 * 2. There are several speed presets that are not supported by all devices and
 *    are determined by set of `DeviceCapabilities::fan...()` methods.
 *
 * 3. Some modern devices also support direct speed setting in percentage.
 *    This capability is determined by the method `DeviceCapabilities::hasFanRaw()`.
 *
 * 4. The fan speed is always `AUTO` in `AUTO` and `DRY` operating modes and cannot be changed.
 */
enum FanSpeed : uint8_t {
  FAN_LOW = 40,    /**< `LOW` fan speed preset. */
  FAN_MEDIUM = 60, /**< `MEDIUM` fan speed preset. */
  FAN_HIGH = 80,   /**< `HIGH` fan speed preset. */
  FAN_AUTO = 102,  /**< `AUTO` fan speed preset. */
};

/**
 * @brief Possible presets. Each operating mode allows only its own set.
 */
enum Preset : uint8_t {
  /**
   * @brief No active preset.
   */
  PRESET_NONE,
  /**
   * @brief `SLEEP` preset.
   *
   * Available in `COOL` and `HEAT` modes. Always supported.
   */
  PRESET_SLEEP,
  /**
   * @brief `TURBO` preset.
   *
   * Available in `COOL` and `HEAT` modes. Support must be confirmed by `DeviceCapabilities::hasTurboCool()` and
   * `DeviceCapabilities::hasTurboHeat()`.
   */
  PRESET_TURBO,
  /**
   * @brief `ECO` preset.
   *
   * Available in `COOL` mode with lower limit of 24°C target temperature. Some
   * models have special advanced `ECO` mode, which operates without 24°C limit and
   * additionally in `AUTO` and `DRY` modes. Support must be confirmed by `DeviceCapabilities::eco()`.
   */
  PRESET_ECO,
  /**
   * @brief `8°C Frost Protection` preset.
   *
   * Available in `HEAT` mode. Support must be confirmed by `DeviceCapabilities::m_eightHot`.
   */
  PRESET_FROST_PROTECTION,
};

/**
 * @brief Units of temperature to display.
 */
enum TemperatureUnit : bool {
  UNIT_CELSIUS,     /**< Celsius degree unit. */
  UNIT_FAHRENHEITS, /**< Fahrenheits degree unit. */
};

}  // namespace ac
}  // namespace midea
