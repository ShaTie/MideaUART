#include <limits>

#include "Midea/AirConditioner/Status/DeviceStatus.hpp"
#include "Midea/Message/PropertiesConsumer.hpp"

namespace midea {
namespace ac {

/* ControllableStatusOld */

template<typename T> inline auto prvTargetTemperature(const T &x) -> uint8_t {
  uint_fast8_t value(x.newTemp + 12);

  if constexpr (std::is_same_v<T, StatusC0>) {
    if (x.newTemp == 0)
      value = x.oldTemp + 16;
  }

  return value * 2 + x.dotTemp;
}

template<typename T> inline auto prvPreset(const T &x) -> Preset {
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

template<NativeStatusConcept T> inline auto ControllableStatusOld::m_update(const T &x) {
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

// Templated update method from `StatusA0` and `StatusC0` messages data.
template<NativeStatusConcept T> inline auto ReadableStatusOld::m_update(const T &x) {
  // Additional `StatusC0` data.
  if constexpr (std::is_same_v<T, StatusC0>) {
    m_indoorTemp = prvInOutTemperature(x.inTemp, x.inTempDec);
    m_outdoorTemp = prvInOutTemperature(x.outTemp, x.outTempDec);
    errInfo = x.errInfo;
    dusFull = x.dusFull;

    imodeResume = x.imodeResume;
    timerMode = x.timerMode;
    test2 = x.test2;
    childSleepMode = x.childSleepMode;
  }

  light = !x.light;
  cosySleep = x.cosySleep;
  save = x.save;
  lowFreqFan = x.lowFreqFan;
  feelOwn = x.feelOwn;
  naturalFan = x.naturalFan;
  dryClean = x.dryClean;
  cleanUp = x.cleanUp;
  exchangeAir = x.exchangeAir;
  nightLight = x.nightLight;
  catchCold = x.catchCold;
  peakElec = x.peakElec;
  setExpand = x.setExpand;
  doubleTemp = x.doubleTemp;
}

template<> inline auto ReadableStatusOld::m_update(const StatusA1 &x) {
  m_indoorTemp = prvInOutTemperature(x.inTemp, 0);
  m_outdoorTemp = prvInOutTemperature(x.outTemp, 0);
}

template<> inline auto ReadableStatusOld::m_update(const StatusC1 &x) {
  // Reads 6-digit BCD number (3 bytes) into an integer.
  auto bcd2uint([](auto bcd) {
    auto k([](auto x) { return x / 16 * 10 + x % 16; });
    return 1'00'00UL * k(bcd[0]) + 1'00UL * k(bcd[1]) + 1UL * k(bcd[2]);
  });
  m_powerUsage = bcd2uint(x.bcdPower) * 0.1F;
}

auto ControllableStatusNew::prvGetDirectionEnum(unsigned x) -> AirFlowDirection {
  if (x >= AIR_DIRECTION_5)
    return AIR_DIRECTION_5;

  if (x >= AIR_DIRECTION_4)
    return AIR_DIRECTION_4;

  if (x >= AIR_DIRECTION_3)
    return AIR_DIRECTION_3;

  if (x >= AIR_DIRECTION_2)
    return AIR_DIRECTION_2;

  if (x >= AIR_DIRECTION_1)
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
