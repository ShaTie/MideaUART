#include "Midea/AirConditioner/Status/DeviceTimers.hpp"

namespace dongle {
namespace midea {
namespace ac {

// High byte base value
static const uint8_t HIGH_BASE(0x7F);

static auto static_get(uint8_t high, uint8_t low) { return 15U * (high - HIGH_BASE) - low; }

static auto static_set(uint8_t &high, unsigned minutes) {
  high = HIGH_BASE;

  if (minutes == 0)
    return 0U;

  minutes += 14;
  high += minutes / 15;

  return 14U - minutes % 15;
}

auto DeviceTimers::getTimeOn() const -> unsigned { return static_get(m_timeOnHigh, m_timeOnLow); }
auto DeviceTimers::getTimeOff() const -> unsigned { return static_get(m_timeOffHigh, m_timeOffLow); }

auto DeviceTimers::setTimeOn(unsigned minutes) -> void { m_timeOnLow = static_set(m_timeOnHigh, minutes); }
auto DeviceTimers::setTimeOff(unsigned minutes) -> void { m_timeOffLow = static_set(m_timeOffHigh, minutes); }

}  // namespace ac
}  // namespace midea
}  // namespace dongle
