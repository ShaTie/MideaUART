#pragma once

#include "Midea/AirConditioner/Status/DeviceStatus.hpp"

namespace midea {
namespace ac {

class DeviceControl : public ControllableStatus {
 public:
  DeviceControl(const DeviceStatus &x) : ControllableStatus(x), m_parent(x) {}

  /**
   * @brief Set target temperature in °C.
   * @param x Temperature value in °C.
   */
  auto setTargetTemp(float x) -> void;

  /**
   * @brief Set target temperature. Unit : 0.5°C.
   * @param x Temperature value. Unit : 0.5°C.
   */
  auto setTargetTempInt(uint8_t x) -> void;

  /**
   * @brief Set power status. Automatically turns on mode change.
   * @param x Power status: `ON/OFF`.
   */
  auto setPowerStatus(bool x) { m_power = x; }

  /**
   * @brief Set operation mode.
   * @param value Operation mode.
   */
  auto setMode(OperationMode value) -> void;

  /**
   * @brief Set vertical swing state.
   * @param x Swing vertical state.
   */
  auto setVerticalSwing(bool x) { m_vSwing = x; }

  /**
   * @brief Set horizontal swing state.
   * @param x Swing horizontal state.
   */
  auto setHorizontalSwing(bool x) { m_hSwing = x; }

  /**
   * @brief Set fan speed in percents if supported, otherwise use `FanSpeed` enum.
   * @param x Fan speed in percents or `FanSpeed` enum value.
   */
  auto setFanSpeed(uint8_t x) -> void;

  /**
   * @brief Set preset.
   * @param x Preset.
   */
  auto setPreset(Preset x) { m_preset = x; }

  /**
   * @brief Set humidity setpoint in `DRY SMART` mode.
   * @param x Humidity setpoint in percents.
   */
  auto setTargetHumidity(uint8_t x) { m_humidity = x; }

  /**
   * @brief Clears air filter maintenance reminder.
   */
  auto clearFilterMaintenance() { m_cleanFanTime = m_parent.airFilterReminder(); }

  auto setStatusQuery() -> DeviceData;

  /**
   * @brief Sets the remaining time of the on timer. Unit : minutes.
   * @param minutes Remaining time in minutes.
   */
  auto setTimeOn(unsigned minutes) { m_timers.setTimeOn(m_power ? 0 : minutes); }

  /**
   * @brief Sets the remaining time of the off timer. Unit : minutes.
   * @param minutes Remaining time in minutes.
   */
  auto setTimeOff(unsigned minutes) { m_timers.setTimeOff(m_power ? minutes : 0); }

 private:
  /// Target temperature correction.
  auto m_tempConstraints() -> void;
  /// Fan speed correction.
  auto m_fanConstraints() -> void;
  /// Device status.
  const DeviceStatus &m_parent;
  /// `Reset Air Filter Maintenance Timer` flag.
  bool m_cleanFanTime;
  /// Old status changed.
  bool m_oldChanged;
};

}  // namespace ac
}  // namespace midea
