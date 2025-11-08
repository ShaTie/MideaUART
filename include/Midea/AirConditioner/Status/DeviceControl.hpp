#pragma once

#include "Midea/AirConditioner/Status/DeviceStatus.hpp"

namespace dongle {
namespace midea {
namespace ac {

class DeviceControl : public ControllableStatus {
 public:
  DeviceControl(const DeviceStatus &x) : ControllableStatus(x), m_parent(x) {}

  /**
   * @brief Set target temperature in °C.
   * @param x Temperature value in °C.
   */
  bool setTargetTemp(float x);

  /**
   * @brief Set target temperature. Unit : 0.5°C.
   * @param x Temperature value. Unit : 0.5°C.
   */
  bool setTargetTempInt(uint8_t x);

  /**
   * @brief Set power status. Automatically turns on mode change.
   * @param x Power status: `ON/OFF`.
   */
  void setPowerStatus(bool x) { m_power = x; }

  /**
   * @brief Set operation mode.
   * @param value Operation mode.
   */
  bool setMode(OperationMode value);

  /**
   * @brief Set vertical swing state.
   * @param x Swing vertical state.
   */
  void setVerticalSwing(bool x) { m_vSwing = x; }

  /**
   * @brief Set horizontal swing state.
   * @param x Swing horizontal state.
   */
  void setHorizontalSwing(bool x) { m_hSwing = x; }

  /**
   * @brief Set fan speed in percents if supported, otherwise use `FanSpeed` enum.
   * @param x Fan speed in percents or `FanSpeed` enum value.
   */
  bool setFanSpeed(uint_fast8_t x);

  /**
   * @brief Set preset.
   * @param x Preset.
   */
  void setPreset(Preset x) { m_preset = x; }

  /**
   * @brief Set humidity setpoint in `DRY SMART` mode.
   * @param x Humidity setpoint in percents.
   */
  void setTargetHumidity(uint8_t x) { m_humidity = x; }

  /**
   * @brief Clears air filter maintenance reminder.
   */
  void clearFilterMaintenance() { m_cleanFanTime = m_parent.airFilterReminder(); }

  DeviceData setStatusQuery() const;

  /**
   * @brief Sets the remaining time of the on timer. Unit : minutes.
   * @param minutes Remaining time in minutes.
   */
  void setTimeOn(unsigned minutes) { m_timers.setTimeOn(m_power ? 0 : minutes); }

  /**
   * @brief Sets the remaining time of the off timer. Unit : minutes.
   * @param minutes Remaining time in minutes.
   */
  void setTimeOff(unsigned minutes) { m_timers.setTimeOff(m_power ? minutes : 0); }

 private:
  /// Fan speed correction.
  bool m_fanConstraints();
  /// Target temp correction.
  bool m_tempConstraints();
  /// Device status.
  const DeviceStatus &m_parent;
  /// `Reset Air Filter Maintenance Timer` flag.
  bool m_cleanFanTime;
  /// Old status changed.
  bool m_oldChanged;
};

}  // namespace ac
}  // namespace midea
}  // namespace dongle
