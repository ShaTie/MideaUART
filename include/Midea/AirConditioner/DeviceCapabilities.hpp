#pragma once

#include "Midea/Message/MideaData.hpp"
#include "Midea/MideaCapabilities.hpp"
#include "Midea/AirConditioner/Status/Types.hpp"

namespace midea {
namespace ac {

/**
 * @brief Capability UUID.
 */
enum CapabilityUUID : PropertyUUID {
  UUID_VWIND = 0x0009,      /**< `Vertical Air Flow Direction`. Values: `1`, `25`, `50`, `75`, `100`. */
  UUID_HWIND = 0x000A,      /**< `Horizontal Air Flow Direction`. Values: `1`, `25`, `50`, `75`, `100`. */
  UUID_HUMIDITY = 0x0015,   /**< `Indoor Humidity`. Status in 0xB1. */
  UUID_SILKY_COOL = 0x0018, /**< `Silky Cool`. Only in `COOL` mode. Values: `0` and `1`. */
  UUID_FEEDBACK = 0x001A,   /**< Beeper feedback in `0xB0`. */

  /**
   * @brief `ECO Intelligent Eye` feature. Not supported in `DRY`, `FAN` and `DRY SMART` modes.
   *
   * The system is controlled intelligently under Intelligent eye mode. It can
   * detect the people’s activities in the room. In cooling mode, when you are
   * away for 30 minutes, the unit will automatically lower the frequency to
   * save energy (for Inverter models only). And the unit will automatically
   * start and resume operation if sensing human activity again.
   */
  UUID_ECO_EYE = 0x0030,
  UUID_WIND_ON_ME = 0x0032,  /**< `Wind ON me`. Only in `COOL` and `HEAT`. Turn on all swing. */
  UUID_WIND_OFF_ME = 0x0033, /**< `Wind OFF me`. Only in `COOL` and `HEAT`. Turn off all swing. */
  UUID_SELF_CLEAN = 0x0039,  /**< `Active Clean`. Values: `0` and `1`. */
  UUID_BREEZE_AWAY = 0x0042, /**< `Breeze Away`. Values: 1 (Off) or 2 (On). Not supported in `AUTO` and `HEAT` modes. */
  UUID_BREEZELESS = 0x0043,  /**< `Breezeless`. Values: 1 (Off), 2 (Away), 3 (Mild), 4 (Less). Only in `COOL` mode. */
  UUID_FAN = 0x0210,         /**< Fan Speeds. Not supported in `AUTO`, `DRY` and `DRY_SMART` modes. */
  UUID_ECO = 0x0212,         /**< `ECO`. */
  UUID_8HEAT = 0x0213,       /**< `8°C Frost Protection`. Only in `HEAT without SLEEP` mode. */
  UUID_MODES = 0x0214,       /**< Supported Modes. */
  UUID_SWING = 0x0215,       /**< Swing Modes. */
  UUID_POWER = 0x0216,       /**< Power Features. */
  UUID_FILTER = 0x0217,      /**< Air Filter Features. (dusFull) */
  UUID_AUX_HEATER = 0x0219,  /**< Electric Auxiliary Heating. */
  UUID_TURBO = 0x021A,       /**< Turbo Mode Features. */
  UUID_DRY = 0x021F,         /**< Drying Modes. */
  UUID_FAHRENHEIT = 0x0222,  /**< Fahrenheit Support. */
  UUID_LIGHT = 0x0224,       /**< LED Control. Values: 0 (Off), 7 (On). */
  UUID_TEMP = 0x0225,        /**< Temperature Ranges. */
  UUID_BUZZER = 0x022C,      /**< Buzzer. Values: 0 (Off), 1 (On). */
  UUID_TWINS = 0x0232,       /**< Slave. Values in 0x0231 B1. */
  UUID_FOUR_DIRECTION = 0x0233, /**< Master. Values in 0x0230 B1. */
};

/*
 * `SLEEP` preset only in `COOL` and `HEAT without FP` modes.
 * Переключение режимов:
 *   1. Сброс всех пресетов кроме `SLEEP` (если режим поддерживается).
 *   2. Если режим `HEAT` и присутствует внешний обогрев (0xB5 `m_dianfure`), то включить в команде флагом
 * `ptcAssis`.
 */

/**
 * @brief Capabilities data.
 *
 */
class DeviceCapabilities : public MideaCapabilities {
  struct TempRange {
    /// Target temperature limit. Unit : 0.5°C.
    uint8_t min{17 * 2}, max{30 * 2};
  };

 public:
  /**
   * @brief Checks if `LOW` fan speed is supported.
   * @return `true` if supported.
   */
  auto hasFanLow() const -> bool { return m_hasFan & 0b00001; }

  /**
   * @brief Checks if `MEDIUM` fan speed is supported.
   * @return `true` if supported.
   */
  auto hasFanMedium() const -> bool { return m_hasFan & 0b00010; }

  /**
   * @brief Checks if `HIGH` fan speed is supported.
   * @return `true` if supported.
   */
  auto hasFanHigh() const -> bool { return m_hasFan & 0b00100; }

  /**
   * @brief Checks if `AUTO` fan speed is supported.
   * @return `true` if supported.
   */
  auto hasFanAuto() const -> bool { return m_hasFan & 0b01000; }

  /**
   * @brief Checks if `RAW` fan speed is supported. Unit : `%`.
   * @return `true` if supported.
   */
  auto hasFanRaw() const -> bool { return m_hasFan & 0b10000; }

  /**
   * @brief Checks if fan speed is supported.
   * @param x Fan speed.
   * @return `true` if supported.
   */
  auto hasFanSpeed(uint_fast8_t x) const -> bool;

  /**
   * @brief `COOL` mode supported.
   * @return
   */
  auto hasModeCool() const -> bool { return m_hasMode & 0b0001; }

  /**
   * @brief `AUTO` mode supported.
   * @return
   */
  auto hasModeAuto() const -> bool { return m_hasMode & 0b0010; }

  /**
   * @brief `HEAT` mode supported.
   * @return
   */
  auto hasModeHeat() const -> bool { return m_hasMode & 0b0100; }

  /**
   * @brief `DRY` mode supported.
   * @return
   */
  auto hasModeDry() const -> bool { return m_hasMode & 0b1000; }

  /**
   * @brief Checks if the `Dry` mode has a smart mode.
   *
   * Smart dehumidification mode based on built-in humidity sensor.
   *
   * @return `true` if the `Dry` mode has a smart mode.
   */
  auto hasDrySmart() const -> bool { return m_hasDrySmart & 0b01; }

  /**
   * @brief Checks if the `Dry Custom` mode is supported.
   *
   * Allows to manually set the humidity level in the room.
   *
   * @return `true` if the `Dry Custom` mode is supported.
   */
  auto hasModeDryCustom() const -> bool { return m_hasDrySmart & 0b10; }

  /**
   * @brief Checks if the built-in humidity sensor can be read.
   * @return `true` if the built-in humidity sensor can be read.
   */
  auto hasIndoorHumidity() const -> bool { return m_hasIndoorHumidity; }

  /**
   * @brief Checks if operation mode is supported.
   * @param mode Operation mode enumeration.
   * @return `true` if operation mode is supported.
   */
  auto hasMode(OperationMode mode) const -> bool;

  /**
   * @brief Gets temperature range in specified operation mode.
   * @param mode Operation mode enumeration.
   * @return Reference to `TempRange` instance.
   */
  auto tempRange(OperationMode mode) const -> const TempRange &;

  /**
   * @brief Gets widest temperature range.
   * @return `TempRange` instance.
   */
  auto tempRangeMax() const -> TempRange;

  /// `COOL TURBO` preset supported.
  auto hasTurboCool() const -> bool { return m_hasTurbo & 0b01; }
  /// `HEAT TURBO` preset supported.
  auto hasTurboHeat() const -> bool { return m_hasTurbo & 0b10; }

  /// `VERTICAL SWING` supported.
  auto hasSwingVertical() const -> bool { return m_hasSwing & 0b01; }
  /// `HORIZONTAL SWING` supported.
  auto hasSwingHorizontal() const -> bool { return m_hasSwing & 0b10; }
  /// `BOTH SWING` supported.
  auto hasSwingBoth() const -> bool { return m_hasSwing == 0b11; }

  /// `POWER USAGE` report supported.
  auto hasPowerReport() const -> bool { return m_hasPower & 0b01; }
  /// `POWER LIMIT` supported.
  auto hasPowerLimits() const -> bool { return m_hasPower & 0b10; }

  /// `AIR FILTER CLEANING REMAINDER` supported.
  auto hasFilterCleanReminder() const -> bool { return m_hasFilter & 0b01; }
  /// `AIR FILTER REPLACEMENT REMAINDER` supported.
  auto hasFilterReplaceReminder() const -> bool { return m_hasFilter & 0b10; }

  /// `ECO` preset supported. Only in `COOL` mode with low limit of target temp `24`.
  auto hasEco() const -> bool { return m_hasEco & 0b01; }
  /// `SPECIAL ECO` preset supported. This mode is similar to `ECO`, but with `AUTO` and `DRY` modes without temp limit.
  auto hasEcoSpecial() const -> bool { return m_hasEco & 0b10; }

  /**
   * @brief Checks if the LED display has an extra decimal place `.5`.
   * @return `true` if the LED display has an extra decimal place `.5`.
   */
  auto hasDotFive() const -> bool { return m_hasTempDot; }

  /**
   * @brief Checks if the LED display has an extra decimal place `.5`.
   * @return `true` if the LED display has an extra decimal place `.5`.
   */
  auto hasFahrenheits() const -> bool { return m_unitChangeable; }

  /**
   * @brief Checks whether the outdoor unit is equipped with an electric heater.
   * @return `true` if the outdoor unit is equipped with an electric heater.
   */
  auto hasElectricHeater() const -> bool { return m_dianfure; }

  /**
   * @brief Dump capabilities report to logs.
   *
   */
  auto dump() const -> void override;

  /**
   * @brief Checks if any of 0xB1 capabilities is supported.
   * @return `true` if any of 0xB1 capabilities is supported.
   */
  auto isQueryNeeded() const -> bool override;

 protected:
  auto m_onProperty(const Property &x) -> void override;
  auto m_temp(const uint8_t *x) -> void;

  /* 0x40/0x41 capabilities */

  /// Supported temperature range in `COOL`, `DRY`, `DRY SMART`, `FAN ONLY` modes.
  TempRange m_tempCool;
  /// Supported temperature range in `AUTO` mode.
  TempRange m_tempAuto;
  /// Supported temperature range in `HEAT` mode.
  TempRange m_tempHeat;

  uint8_t m_hasMode : 4;
  uint8_t m_hasDrySmart : 2;
  uint8_t m_hasSwing : 2;

  uint8_t m_hasEco : 2;
  uint8_t m_hasTurbo : 2;
  uint8_t m_hasPower : 2;
  uint8_t m_hasFilter : 2;

  uint8_t m_hasFan : 5;

  /// LED screen has decimal point.
  bool m_hasTempDot : 1;
  /// `FAHRENHEITS` display units supported.
  bool m_unitChangeable : 1;
  /// LED Light. Values: 0 (Off), 7 (On).
  bool m_hasLight : 1;
  /// `FROST PROTECTION` preset supported.
  bool m_eightHot : 1;
  /// `ELECTRIC AUXILIARY HEATER` supported.
  bool m_dianfure : 1;

  /* 0xB0/0xB1 capabilities */

  /// `ACTIVE CLEAN` preset supported.
  bool m_hasSelfClean : 1;
  /// `SILKY COOL` preset supported.
  bool m_hasNoWindFeel : 1;
  /// `BREEZE AWAY` preset supported. Not supported in `AUTO` and `HEAT` modes.
  bool m_hasOneKeyNoWindOnMe : 1;
  /// `BREEZELESS` preset supported.
  bool m_hasBreeze : 1;
  /// `SOUND` supported.
  bool m_hasBuzzer : 1;
  /// `ECO INTELLIGENT EYE` preset supported.
  bool m_hasSmartEye : 1;
  /// `INDOOR HUMIDITY` report supported.
  bool m_hasIndoorHumidity : 1;
  /// `VERTICAL AIR FLOW DIRECTION` supported.
  bool m_hasVerticalWind : 1;
  /// `HORIZONTAL AIR FLOW DIRECTION` supported.
  bool m_hasHorizontalWind : 1;
  /// Is multi-split system.
  bool m_isTwins : 1;
  /// Cassette four direction type.
  bool m_isFourDirection : 1;
  /// `WIND ON ME` preset supported.
  bool m_hasBlowingPeople : 1;
  /// `WIND OFF ME` preset supported.
  bool m_hasAvoidPeople : 1;
};

}  // namespace ac
}  // namespace midea
