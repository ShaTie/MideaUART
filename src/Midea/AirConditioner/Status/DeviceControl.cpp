#include "Midea/AirConditioner/Status/DeviceControl.hpp"
#include "Dongle/Message/Message.hpp"

namespace midea {
namespace ac {

auto DeviceControl::setMode(OperationMode value) -> bool {
  if (m_power && value == m_mode)
    return false;

  if (!m_parent.hasMode(value))
    return false;

  m_power = true;
  m_mode = value;
  m_preset = PRESET_NONE;
  m_oldChanged = true;

  return true;
}

auto DeviceControl::setTargetTemp(float x) -> bool { return setTargetTempInt(x * 2 + 0.5F); }

auto DeviceControl::setTargetTempInt(uint8_t value) -> bool {
  if (value == m_targetTemp)
    return false;

  m_targetTemp = value;
  m_oldChanged = true;

  return true;
}

auto DeviceControl::setFanSpeed(uint_fast8_t value) -> bool {
  if (value == m_fanSpeed)
    return false;

  if (!m_parent.hasFanSpeed(value))
    return false;

  m_fanSpeed = value;
  m_preset = PRESET_NONE;
  m_oldChanged = true;

  return true;
}

auto DeviceControl::m_tempConstraints() -> bool {
  auto &range(m_parent.tempRange(m_mode));

  if (m_targetTemp < range.min)
    m_targetTemp = range.min;

  else if (m_targetTemp > range.max)
    m_targetTemp = range.max;

  else
    return false;

  return true;
}

auto DeviceControl::m_fanConstraints() -> bool {
  switch (m_mode) {
    case MODE_AUTO:
    case MODE_DRY:
    case MODE_DRY_CUSTOM:
      return setFanSpeed(FAN_AUTO);
    default:
      return false;
  }
}

auto DeviceControl::setStatusQuery() const -> DeviceData {
  // `old_temp` (4-bits) supports range from 17°C (1) to 30°C (14).
  // If the value is outside this range, `old_temp` is equal to the minimum limit of 17°C (1).
  // `new_temp` (5-bits) supports more wide range from 13°C (1) to 42°C (30).
  bool dot_temp(m_targetTemp % 2);
  uint_fast8_t new_temp(m_targetTemp / 2 - 12), old_temp(new_temp - 5);

  if (old_temp > 13)
    old_temp = 0;

  // Set PTC Assist flag if in HEAT mode and electric heater is supported.
  bool ptc_assis(m_mode == MODE_HEAT && m_parent.hasElectricHeater());

  Message data(MSG_CONTROL, 23);
  auto &s(m_parent);

  auto shl([](auto value, auto n_bits) { return value << n_bits; });

  data[0] = 0x40;

  data[1] = shl(m_feedback, 6) + shl(s.test2, 5) + shl(s.timerMode, 4) + shl(s.childSleepMode, 3) +
            shl(s.imodeResume, 2) + shl(true, 1) + shl(m_power, 0);

  data[2] = shl(m_mode, 5) + shl(dot_temp, 4) + shl(old_temp + 1, 0);
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
