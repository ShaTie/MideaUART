#include "Midea/AirConditioner/Status/DeviceTimers.hpp"

namespace dongle {
namespace midea {
namespace ac {

// High byte base value
static const uint8_t HIGH_BASE(0x7F);

static auto prvGetTime(uint8_t high, uint8_t low) { return 15U * (high - HIGH_BASE) - low; }

static auto prvSetTime(uint8_t &high, unsigned minutes) {
  high = HIGH_BASE;

  if (minutes == 0)
    return 0U;

  minutes += 14;
  high += minutes / 15;

  return 14U - minutes % 15;
}

auto DeviceTimers::getTimeOn() const -> unsigned { return prvGetTime(m_timeOnHigh, m_timeOnLow); }
auto DeviceTimers::getTimeOff() const -> unsigned { return prvGetTime(m_timeOffHigh, m_timeOffLow); }

auto DeviceTimers::setTimeOn(unsigned minutes) -> void { m_timeOnLow = prvSetTime(m_timeOnHigh, minutes); }
auto DeviceTimers::setTimeOff(unsigned minutes) -> void { m_timeOffLow = prvSetTime(m_timeOffHigh, minutes); }

}  // namespace ac
}  // namespace midea
}  // namespace dongle
