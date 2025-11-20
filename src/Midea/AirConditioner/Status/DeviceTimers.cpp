#include "Midea/AirConditioner/Status/DeviceTimers.hpp"

namespace midea {
namespace ac {

// High byte base value
static constexpr auto HIGH_BASE{0x7F};

static auto prvGetTime(auto high, auto low) { return 15U * (high - HIGH_BASE) - low; }

static auto prvSetTime(auto &high, auto minutes) {
  high = HIGH_BASE;

  if (minutes == 0)
    return 0U;

  minutes += 14;
  high += minutes / 15;

  return 14U - minutes % 15;
}

auto DeviceTimers::getTimeOn() const -> unsigned { return prvGetTime(m_timeOnHigh, m_timeOnLow); }
auto DeviceTimers::setTimeOn(unsigned minutes) -> void { m_timeOnLow = prvSetTime(m_timeOnHigh, minutes); }

auto DeviceTimers::getTimeOff() const -> unsigned { return prvGetTime(m_timeOffHigh, m_timeOffLow); }
auto DeviceTimers::setTimeOff(unsigned minutes) -> void { m_timeOffLow = prvSetTime(m_timeOffHigh, minutes); }

}  // namespace ac
}  // namespace midea
