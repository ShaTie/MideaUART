#pragma once

#include <cstdint>

namespace midea {
namespace ac {

/**
 * @brief Device timer class. Stores raw data as it is transmitted in a message.
 */
class DeviceTimers {
 public:
  /**
   * @brief Default constructor. Initializes both timers as inactive.
   */
  DeviceTimers() : m_timeOnHigh(0x7F), m_timeOffHigh(0x7F), m_timeOffLow(0), m_timeOnLow(0) {}

  /**
   * @brief Constructor with initial timer values.
   * @param onMinutes Initial on timer value in minutes.
   * @param offMinutes Initial off timer value in minutes.
   */
  DeviceTimers(unsigned onMinutes, unsigned offMinutes) { setTimeOn(onMinutes), setTimeOff(offMinutes); }

  /**
   * @brief Checks if the on timer is active.
   * @return `true` if the on timer is active.
   */
  auto isTimeOn() const -> bool { return m_timeOnHigh & 0x80; }

  /**
   * @brief Gets the remaining time of the on timer. Unit : minutes.
   * @return Returns the remaining time in minutes.
   */
  auto getTimeOn() const -> unsigned;

  /**
   * @brief Sets the remaining time of the on timer. Unit : minutes.
   * @param minutes Remaining time in minutes.
   */
  auto setTimeOn(unsigned minutes) -> void;

  /**
   * @brief Checks if the off timer is active.
   * @return `true` if the off timer is active.
   */
  auto isTimeOff() const -> bool { return m_timeOffHigh & 0x80; }

  /**
   * @brief Gets the remaining time of the off timer. Unit : minutes.
   * @return Returns the remaining time in minutes.
   */
  auto getTimeOff() const -> unsigned;

  /**
   * @brief Sets the remaining time of the off timer. Unit : minutes.
   * @param minutes Remaining time in minutes.
   */
  auto setTimeOff(unsigned minutes) -> void;

 private:
  uint8_t m_timeOnHigh;
  uint8_t m_timeOffHigh;
  uint8_t m_timeOffLow : 4;
  uint8_t m_timeOnLow : 4;
};

}  // namespace ac
}  // namespace midea
