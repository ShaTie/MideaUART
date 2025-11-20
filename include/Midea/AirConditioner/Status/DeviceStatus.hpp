#pragma once

#include <array>

#include "Midea/AirConditioner/DeviceCapabilities.hpp"
#include "Midea/AirConditioner/DeviceData.hpp"
#include "Midea/AirConditioner/Status/DeviceTimers.hpp"

namespace midea {
namespace ac {

class StatusA0;
class StatusA1;
class StatusC0;
class StatusC1;

/**
 * @brief Device control settings. Stored in the device instance, copied to the control object instance to allow for
 * quick redefinition without global changes.
 */
class DeviceControlSettings {
 public:
  /**
   * @brief Sets beeper sound feedback when controlling.
   * @param x `true` to enable beeper feedback, `false` to disable.
   */
  auto setControlFeedback(bool x) { m_feedback = x; }

  /**
   * @brief Gets beeper sound feedback setting.
   * @return `true` if beeper feedback is enabled, `false` otherwise.
   */
  auto getControlFeedback() const { return m_feedback; }

  /**
   * @brief Sets temperature display unit.
   * @param x Temperature unit.
   */
  auto setDisplayUnit(TemperatureUnit x) { m_tempUnit = x; }

  /**
   * @brief Gets temperature display unit.
   * @return Temperature unit.
   */
  auto getDisplayUnit() const { return m_tempUnit; }

 protected:
  /// Beeper control feedback.
  bool m_feedback : 1;
  /// Display temperature in `Fahrenheits`.
  TemperatureUnit m_tempUnit : 1;
};

/**
 * @brief Device status obtained from messages `C0`, `A0`, `A1`. Controlled via message `40`. Stored in the device
 * instance, copied to the control object instance.
 */
class ControllableStatusOld {
 public:
  /**
   * @brief Get target temperature in °C.
   * @return Temperature value in °C.
   */
  auto getTargetTemp() const { return m_targetTemp * 0.5F; }

  /**
   * @brief Get target temperature. Unit : 0.5°C.
   * @return Temperature value. Unit : 0.5°C.
   */
  auto getTargetTempInt() const { return m_targetTemp; }

  /**
   * @brief Get power status.
   * @return Power status: `ON/OFF`.
   */
  auto getPowerStatus() const { return m_power; }

  /**
   * @brief Get operation mode.
   * @return Operation mode.
   */
  auto getMode() const { return m_mode; }

  /**
   * @brief Get horizontal swing state.
   * @return Horizontal swing state.
   */
  auto getHorizontalSwing() const { return m_hSwing; }

  /**
   * @brief Get vertical swing state.
   * @return Vertical swing state.
   */
  auto getVerticalSwing() const { return m_vSwing; }

  /**
   * @brief Get fan speed in percents.
   * @return Fan speed in percents.
   */
  auto getFanSpeed() const { return m_fanSpeed; }

  /**
   * @brief Get fan speed as `FanSpeed` enum value.
   * @return Fan speed as `FanSpeed` enum value.
   */
  auto getFanSpeedEnum() const -> FanSpeed;

  /**
   * @brief Get preset.
   * @return Preset.
   */
  auto getPreset() const { return m_preset; }

  /**
   * @brief Get humidity setpoint in `DRY SMART` mode.
   * @return Humidity setpoint in percents.
   */
  auto getTargetHumidity() const { return m_humidity; }

  /**
   * @brief Gets the remaining time of the on timer. Unit : minutes.
   * @return Returns the remaining time in minutes.
   */
  auto getTimeOn() const { return m_timers.getTimeOn(); }

  /**
   * @brief Gets the remaining time of the off timer. Unit : minutes.
   * @return Returns the remaining time in minutes.
   */
  auto getTimeOff() const { return m_timers.getTimeOff(); }

  /**
   * @brief Checks if the on timer is active.
   * @return `true` if the on timer is active.
   */
  auto isTimeOn() const { return m_timers.isTimeOn(); }

  /**
   * @brief Checks if the off timer is active.
   * @return `true` if the off timer is active.
   */
  auto isTimeOff() const { return m_timers.isTimeOff(); }

 protected:
  /// Power status: `ON/OFF`.
  bool m_power : 1;
  /// Operation mode.
  OperationMode m_mode : 3;
  /// Horizontal swing mode.
  bool m_hSwing : 1;
  /// Vertical swing mode.
  bool m_vSwing : 1;
  /// Target temperature. Unit : 0.5°C.
  uint8_t m_targetTemp;
  /// Fan speed.
  uint8_t m_fanSpeed;
  /// Preset.
  Preset m_preset;
  /// Humidity setpoint in `Smart Dry` mode. Fan speed must be `AUTO`.
  uint8_t m_humidity;
  /// ON/OFF timers.
  DeviceTimers m_timers;

  /* inline update methods used in `DeviceStatus` class */
  auto m_update(const auto &x);
  auto m_update(const StatusA1 &a1);
};

/**
 * @brief Device status obtained from messages `C0`, `C1`, `A0`, `A1`. Read-only. Stored in the device instance.
 */
class ReadableStatusOld {
  friend class DeviceControl;

 public:
  /**
   * @brief Get indoor temperature in °C.
   * @return Indoor temperature in °C.
   */
  auto getIndoorTemp() const { return m_indoorTemp; }

  /**
   * @brief Get outdoor temperature in °C.
   * @return Outdoor temperature in °C.
   */
  auto getOutdoorTemp() const { return m_outdoorTemp; }

  /**
   * @brief Air filter maintenance reminder status.
   * @return Reminder status.
   */
  auto airFilterReminder() const { return dusFull; }

  /**
   * @brief Gets LED display status.
   * @return LED display status.
   */
  auto ledDisplayStatus() const { return light; }

 private:
  /// Real-time power. Unit : Watt.
  float m_powerUsage;
  /// Indoor temperature. Unit : °C.
  float m_indoorTemp;
  /// Outdoor temperature. Unit : °C.
  float m_outdoorTemp;
  /// Indoor humidity. Unit : %. Query via `0xB1` message.
  uint8_t m_indoorHumidity;
  /// Error code. Known: `0x26` - `Water Full`.
  uint8_t errInfo;
  /// LED display status. Originally 3 bits : 0 (on), 7 (off).
  bool light : 1;
  /// `Air Filter Maintenance` flag.
  bool dusFull : 1;

  // UNKNOWN FLAGS

  bool imodeResume : 1;
  bool timerMode : 1;
  bool test2 : 1;

  uint8_t cosySleep : 2;
  bool save : 1;
  bool lowFreqFan : 1;
  bool feelOwn : 1;

  bool childSleepMode : 1;
  bool naturalFan : 1;
  bool dryClean : 1;
  bool cleanUp : 1;

  bool exchangeAir : 1;
  bool nightLight : 1;
  bool catchCold : 1;
  bool peakElec : 1;

  uint8_t setExpand : 6;
  bool doubleTemp : 1;

 protected:
  /* inline update methods used in `DeviceStatus` class */
  auto m_update(const StatusA0 &x);
  auto m_update(const StatusA1 &x);
  auto m_update(const StatusC0 &x);
  auto m_update(const StatusC1 &x);
};

/**
 * @brief Device status obtained from message `B1`. Controlled via message `B0`. Stored in the device instance, copied
 * to the control object instance.
 */
struct ControllableStatusNew : public PropertiesConsumer {
  /// Master Values.
  std::array<uint8_t, 4> mMasterValues{};

  /// Slave Values.
  std::array<uint8_t, 4> mSlaveValues{};

  /// Horizontal Air Flow Direction. Values: 1, 25, 50, 75, 100.
  uint8_t hWindDirection{};

  /// Vertical Air Flow Direction. Values: 1, 25, 50, 75, 100.
  uint8_t vWindDirection{};

  /// Breezeless. Values: 1 (Off), 2 (Away), 3 (Mild), 4 (Less).
  BreezelessMode breezelessMode{};

  /// `BUZZER` state.
  bool isBuzzerOn : 1;

  /// `ACTIVE CLEAN` state.
  bool isSelfCleanOn : 1;

  /// `SILKY COOL` status.
  bool isSilkyCoolOn : 1;

  /// `WIND ON ME` status. Only in `COOL` and `HEAT`. Turn ON all Swing.
  bool isWindOnMeOn : 1;

  /// `WIND OFF ME` status. Only in `COOL` and `HEAT`. Turn OFF all Swing.
  bool isWindOffMeOn : 1;

  /// `BREEZE AWAY` state.
  bool isBreezeAwayOn : 1;

  /// `ECO Intelligent EYE` state.
  bool isSmartEyeOn : 1;

  void m_onProperty(const Property &x) override;
  static AirFlowDirection getDirectionEnum(unsigned x);
};

/**
 * @brief Aggregation of controllable status types, that must be copied to control instance object.
 */
class ControllableStatus : public ControllableStatusOld, public ControllableStatusNew, public DeviceControlSettings {};

/**
 * @brief
 */
class DeviceStatus : public ControllableStatus, public ReadableStatusOld, public DeviceCapabilities {
 protected:
  bool m_update(const MideaData &x);
};

}  // namespace ac
}  // namespace midea
