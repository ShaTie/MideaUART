#include <algorithm>

#include "Midea/AirConditioner/Status/DeviceControl.hpp"
#include "Dongle/Message/Message.hpp"

namespace midea {
namespace ac {

auto DeviceControl::setMode(OperationMode mode) -> void {
  // ignore if already powered on and mode not changes
  if (m_power && mode == m_mode)
    return;

  if (!m_parent.hasMode(mode))
    return;

  m_mode = mode;
  m_preset = PRESET_NONE;
  m_power = true;
  m_oldChanged = true;
}

auto DeviceControl::setTargetTemp(float x) -> void { setTargetTempInt(x * 2 + 0.5F); }

auto DeviceControl::setTargetTempInt(uint8_t value) -> void {
  if (value == m_targetTemp)
    return;

  m_targetTemp = value;
  m_oldChanged = true;
}

auto DeviceControl::setFanSpeed(uint_fast8_t value) -> void {
  if (value == m_fanSpeed)
    return;

  if (!m_parent.hasFanSpeed(value))
    return;

  m_fanSpeed = value;
  m_preset = PRESET_NONE;
  m_oldChanged = true;
}

inline auto DeviceControl::m_fanConstraints() -> void {
  switch (m_mode) {
    case MODE_AUTO:
    case MODE_DRY:
    case MODE_DRY_CUSTOM:
      setFanSpeed(FAN_AUTO);
    default:
      break;
  }
}

auto DeviceControl::setStatusQuery() const -> DeviceData {
  // `old_temp` (4-bits) supports range from 17°C (1) to 30°C (14).
  // If the value is outside this range, `old_temp` is equal to the minimum limit of 17°C (1).
  // `new_temp` (5-bits) supports more wide range from 13°C (1) to 42°C (30).
  auto &range(m_parent.tempRange(m_mode));
  const uint_fast8_t temp(std::clamp(m_targetTemp, range.min, range.max)), dot_temp(temp % 2);
  const uint_fast8_t new_temp(temp / 2 - 12), old_temp(std::clamp<uint_fast8_t>(temp / 2, 17, 30) - 16);

  // Set PTC Assist flag if in HEAT mode and electric heater is supported.
  bool ptc_assis(m_mode == MODE_HEAT && m_parent.hasElectricHeater());

  Message data(MSG_CONTROL, 23);
  auto &s(m_parent);

  auto shl([](auto value, auto n_bits) { return value << n_bits; });

  data[0] = 0x40;

  data[1] = shl(m_feedback, 6) + shl(s.test2, 5) + shl(s.timerMode, 4) + shl(s.childSleepMode, 3) +
            shl(s.imodeResume, 2) + shl(true, 1) + shl(m_power, 0);

  data[2] = shl(m_mode, 5) + shl(dot_temp, 4) + shl(old_temp, 0);
  data[3] = shl(m_fanSpeed, 0);

  data.ref<DeviceTimers>(4) = m_timers;

  data[7] = 0x30 + (m_vSwing ? 0b1100 : 0) + (m_hSwing ? 0b0011 : 0);

  data[8] = shl(s.feelOwn, 7) + shl(m_preset == PRESET_TURBO, 5) + shl(s.lowFreqFan, 4) + shl(s.save, 3) +
            shl(s.cosySleep, 0);

  data[9] = shl(m_preset == PRESET_ECO, 7) + shl(s.cleanUp, 5) + shl(ptc_assis, 3) + shl(s.dryClean, 2) +
            shl(s.exchangeAir, 1);

  data[10] = shl(m_cleanFanTime, 7) + shl(s.dusFull, 6) + shl(s.peakElec, 5) + shl(s.nightLight, 4) +
             shl(s.catchCold, 3) + shl(m_tempUnit, 2) + shl(m_preset == PRESET_TURBO, 1) +
             shl(m_preset == PRESET_SLEEP, 0);

  data[15] = shl(s.naturalFan, 6);
  data[18] = new_temp;
  data[19] = m_humidity;
  data[21] = shl(m_preset == PRESET_FROST_PROTECTION, 7) + shl(s.doubleTemp, 6) + shl(s.setExpand, 0);

  return DeviceData(23);  // NRVO optimization? Or may be use move semantics?
}

}  // namespace ac
}  // namespace midea
