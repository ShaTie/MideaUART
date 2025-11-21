#include <limits>

#include "Midea/AirConditioner/Status/DeviceStatus.hpp"
#include "Midea/AirConditioner/Status/DeviceStatusNative.hpp"
#include "Midea/Message/PropertiesConsumer.hpp"

namespace midea {
namespace ac {

/* ControllableStatusOld */

static auto prvTargetTemperature(const StatusC0 &c0) -> uint8_t {
  uint_fast8_t value(c0.newTemp ? (c0.newTemp + 12) : (c0.oldTemp + 16));
  return value * 2 + c0.dotTemp;
}

static auto prvTargetTemperature(const StatusA0 &a0) -> uint8_t { return 24 + a0.newTemp * 2 + a0.dotTemp; }

static auto prvPreset(const auto &x) -> Preset {
  if (x.sleepFunc)
    return PRESET_SLEEP;

  if (x.turbo1 || x.turbo2)
    return PRESET_TURBO;

  if (x.eco)
    return PRESET_ECO;

  if (x.eightHot)
    return PRESET_FROST_PROTECTION;

  return PRESET_NONE;
}

template<typename T> auto ControllableStatusOld::m_update(const T &x) {
  m_humidity = x.humidity;

  if constexpr (!std::is_same_v<T, StatusA1>) {
    m_power = x.power;
    m_mode = x.mode;
    m_hSwing = static_cast<bool>(x.leftRightFan);
    m_vSwing = static_cast<bool>(x.updownFan);
    m_targetTemp = prvTargetTemperature(x);
    m_fanSpeed = x.fanSpeed;
    m_timers = x.timers;
    m_preset = prvPreset(x);
  }
}

auto ControllableStatusOld::getFanSpeedEnum() const -> FanSpeed {
  if (m_fanSpeed <= 50)
    return FAN_LOW;

  if (m_fanSpeed < 80)
    return FAN_MEDIUM;

  if (m_fanSpeed <= 100)
    return FAN_HIGH;

  return FAN_AUTO;
}

/* ReadableStatusOld */

/**
 * @brief Converts indoor/outdoor temperature raw value and BCD decimal part to float value in °C.
 * @param value Raw unsigned 8-bits integer value in `0.5°C` with `+25°C` offset.
 * If equal to `0xFF`, the value is `NaN`. Due to this restrictions, minimal supported temperature is `-25.0°C`.
 * @param decimal 4-bits unsigned BCD decimal part. Increase accuracy from `0.5°C` to `0.1°C`.
 * If not supported, always equals `0`.
 * @return Temperature in °C.
 */
static auto prvInOutTemperature(int value, int decimal) -> float {
  // Checking for `NaN`
  if (value == 0xFF)
    return std::numeric_limits<float>::quiet_NaN();

  // Unit : 0.5°C => 0.1°C
  value *= 5;

  // .5°C is already in value, subtract it from BCD decimal to compensate
  if (decimal >= 5)
    decimal -= 5;

  // Subtract +25.0°C offset and correct sign of decimal
  if ((value -= 250) < 0)
    decimal = -decimal;

  return (value + decimal) * 0.1F;
}

inline auto ReadableStatusOld::m_update(const StatusA0 &a0) {
  light = !a0.light;

  // STORE UNKNOWN FLAGS

  cosySleep = a0.cosySleep;
  save = a0.save;
  lowFreqFan = a0.lowFreqFan;
  feelOwn = a0.feelOwn;
  naturalFan = a0.naturalFan;
  dryClean = a0.dryClean;
  cleanUp = a0.cleanUp;
  exchangeAir = a0.exchangeAir;
  nightLight = a0.nightLight;
  catchCold = a0.catchCold;
  peakElec = a0.peakElec;
  setExpand = a0.setExpand;
  doubleTemp = a0.doubleTemp;
}

inline auto ReadableStatusOld::m_update(const StatusA1 &a1) {
  m_indoorTemp = prvInOutTemperature(a1.inTemp, 0);
  m_outdoorTemp = prvInOutTemperature(a1.outTemp, 0);
}

inline auto ReadableStatusOld::m_update(const StatusC0 &c0) {
  m_indoorTemp = prvInOutTemperature(c0.inTemp, c0.inTempDec);
  m_outdoorTemp = prvInOutTemperature(c0.outTemp, c0.outTempDec);
  errInfo = c0.errInfo;
  dusFull = c0.dusFull;
  light = !c0.light;

  // STORE UNKNOWN FLAGS

  imodeResume = c0.imodeResume;
  timerMode = c0.timerMode;
  test2 = c0.test2;
  cosySleep = c0.cosySleep;
  save = c0.save;
  lowFreqFan = c0.lowFreqFan;
  feelOwn = c0.feelOwn;
  childSleepMode = c0.childSleepMode;
  naturalFan = c0.naturalFan;
  dryClean = c0.dryClean;
  cleanUp = c0.cleanUp;
  exchangeAir = c0.exchangeAir;
  nightLight = c0.nightLight;
  catchCold = c0.catchCold;
  peakElec = c0.peakElec;
  setExpand = c0.setExpand;
  doubleTemp = c0.doubleTemp;
}

inline auto ReadableStatusOld::m_update(const StatusC1 &c1) {
  // Reads 6-digit BCD number (3 bytes) into an integer.
  auto bcd2uint([](auto bcd) {
    auto k([](auto x) { return x / 16 * 10 + x % 16; });
    return 1'00'00UL * k(bcd[0]) + 1'00UL * k(bcd[1]) + 1UL * k(bcd[2]);
  });
  m_powerUsage = bcd2uint(c1.bcdPower) * 0.1F;
}

auto ControllableStatusNew::getDirectionEnum(unsigned x) -> AirFlowDirection {
  if (x >= 100)
    return AIR_DIRECTION_5;

  if (x >= 75)
    return AIR_DIRECTION_4;

  if (x >= 50)
    return AIR_DIRECTION_3;

  if (x >= 25)
    return AIR_DIRECTION_2;

  if (x >= 1)
    return AIR_DIRECTION_1;

  return AIR_DIRECTION_UNKNOWN;
}

/* DeviceStatus */

auto DeviceStatus::m_update(const MideaData &x) -> bool {
  switch (x.typeID()) {
    case 0xC0: {
      auto &s(x.ref<StatusC0>());
      ControllableStatusOld::m_update(s);
      ReadableStatusOld::m_update(s);
      return true;
    }

    case 0xA0: {
      auto &s(x.ref<StatusA0>());
      ControllableStatusOld::m_update(s);
      ReadableStatusOld::m_update(s);
      return true;
    }

    case 0xA1: {
      auto &s(x.ref<StatusA1>());
      ControllableStatusOld::m_update(s);
      ReadableStatusOld::m_update(s);
      return true;
    }

    case 0xC1: {
      auto &s(x.ref<StatusC1>());
      ReadableStatusOld::m_update(s);
      return true;
    }

    default:
      return false;
  }
}

auto ac::ControllableStatusNew::m_onProperty(const Property &x) -> void {
  unsigned b0(*x.data());
  switch (x.uuid()) {
    case UUID_VWIND:
      vWindDirection = b0;
      break;
    case UUID_HWIND:
      hWindDirection = b0;
      break;
    case UUID_BREEZELESS:
      breezelessMode = BreezelessMode(b0);
      break;
    case UUID_BUZZER:
      isBuzzerOn = !!b0;
      break;
  }
}

}  // namespace ac
}  // namespace midea
