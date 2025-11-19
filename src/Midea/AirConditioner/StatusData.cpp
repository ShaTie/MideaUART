#include "Midea/AirConditioner/StatusData.hpp"
#include "Midea/AirConditioner/DeviceCapabilities.hpp"

namespace midea {
namespace ac {

float StatusData::getTargetTemp() const {
  float temp = static_cast<float>(getValue(2, 15) + 16);

  if (getValue(2, 16))
    temp += 0.5f;

  return temp;
}

void StatusData::setTargetTemp(float temp) {
  uint8_t tmp = static_cast<uint8_t>(temp * 16.0f) + 4;
  setValue(2, ((tmp & 8) << 1) | (tmp >> 4), 31);
}

static float i16tof(int16_t in) { return static_cast<float>(in - 50) * 0.5f; }
float StatusData::getIndoorTemp() const { return i16tof(getValue(11)); }
float StatusData::getOutdoorTemp() const { return i16tof(getValue(12)); }
float StatusData::getHumiditySetpoint() const { return static_cast<float>(getValue(19, 127)); }

OperationMode StatusData::getMode() const { return m_getPower() ? this->getRawMode() : OperationMode::MODE_UNKNOWN; }

void StatusData::setMode(OperationMode mode) {
  if (mode == OperationMode::MODE_UNKNOWN) {
    m_setPower(false);
    return;
  }

  m_setPower(true);
  setValue(2, mode, 7, 5);
}

FanSpeed StatusData::getFanMode() const {
  const auto speed = static_cast<FanSpeed>(getValue(3));

  if (speed <= FanSpeed::FAN_LOW)
    return FanSpeed::FAN_LOW;

  if (speed <= FanSpeed::FAN_MEDIUM)
    return FanSpeed::FAN_MEDIUM;

  return speed;
}

Preset StatusData::getPreset() const {
  if (m_getEco())
    return Preset::PRESET_ECO;

  if (m_getTurbo())
    return Preset::PRESET_TURBO;

  if (m_getSleep())
    return Preset::PRESET_SLEEP;

  if (m_getFreezeProtection())
    return Preset::PRESET_FROST_PROTECTION;

  return Preset::PRESET_NONE;
}

void StatusData::setPreset(Preset preset) {
  m_setEco(false);
  m_setSleep(false);
  m_setTurbo(false);
  m_setFreezeProtection(false);

  switch (preset) {
    case Preset::PRESET_NONE:
      break;

    case Preset::PRESET_ECO:
      m_setEco(true);
      break;

    case Preset::PRESET_TURBO:
      m_setTurbo(true);
      break;

    case Preset::PRESET_SLEEP:
      m_setSleep(true);
      break;

    case Preset::PRESET_FROST_PROTECTION:
      m_setFreezeProtection(true);
      break;

    default:
      break;
  }
}

static uint8_t bcd2u8(uint8_t x) { return x / 16 * 10 + x % 16; }

float StatusData::getPowerUsage() const {
  unsigned long power = 0, weight = 1;

  for (auto it = &m_data[18];; weight *= 100, --it) {
    power += bcd2u8(*it) * weight;

    if (weight == 10000)
      return static_cast<float>(power) * 0.1f;
  }
}

/* PropertiesStateQuery */

PropertiesStateQuery::PropertiesStateQuery() : MideaData{0xB1} {
  this->appendUUID(UUID_HUMIDITY);
  this->appendUUID(UUID_SILKY_COOL);
  this->appendUUID(UUID_FEEDBACK);
  this->appendUUID(UUID_ECO_EYE);
  this->appendUUID(UUID_SELF_CLEAN);
  this->appendUUID(UUID_BREEZE_AWAY);
  this->appendUUID(UUID_BREEZELESS);
  this->appendUUID(UUID_BUZZER);
  // this->appendChecksum();
}

PropertiesStateQuery::PropertiesStateQuery(const DeviceCapabilities &s) : MideaData{0xB1} {
  /*
  if (s.m_hasBlowingPeople)
    this->appendUUID(UUID_WIND_ON_ME);

  if (s.m_hasAvoidPeople)
    this->appendUUID(UUID_WIND_OFF_ME);

  if (s.m_hasSelfClean)
    this->appendUUID(UUID_SELF_CLEAN);

  if (s.m_hasOneKeyNoWindOnMe)
    this->appendUUID(UUID_BREEZE_AWAY);

  if (s.m_hasBreeze)
    this->appendUUID(UUID_BREEZELESS);

  if (s.m_hasSmartEye)
    this->appendUUID(UUID_ECO_EYE);

  if (s.m_hasBuzzer)
    this->appendUUID(UUID_BUZZER);

  if (s.hasDrySmartAuto() || s.hasModeDryCustom())
    this->appendUUID(UUID_HUMIDITY);

  if (s.m_hasVerticalWind)
    this->appendUUID(UUID_VWIND);

  if (s.m_hasHorizontalWind)
    this->appendUUID(UUID_HWIND);

  if (s.m_isTwins)
    this->appendUUID(2 * 256 + 49);

  if (s.m_isFourDirection)
    this->appendUUID(2 * 256 + 48);

  this->appendChecksum();
  */
}

}  // namespace ac
}  // namespace midea
