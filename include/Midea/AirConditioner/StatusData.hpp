#pragma once

#include <cstring>

#include "Midea/Message/MideaData.hpp"
#include "Midea/Message/PropertiesConsumer.hpp"
#include "Helpers/Helpers.hpp"
#include "Midea/AirConditioner/Status/DeviceStatus.hpp"

namespace dongle {
namespace midea {
namespace ac {

/**
 * @brief Swing modes enumeration.
 */
enum SwingMode : uint8_t {
  /** @brief Swing mode turned off state. */
  SWING_OFF,
  /**
   * @brief Horizontal (left-and-right) swing mode. Support must be confirmed by
   * `DeviceCapabilities::hasSwingHorizontal()`.
   */
  SWING_HORIZONTAL = 0b0011,
  /**
   * @brief Vertical (up-and-down) swing mode. Support must be confirmed by `DeviceCapabilities::hasSwingVertical()`.
   */
  SWING_VERTICAL = 0b1100,
  /**
   * @brief Both vertical (up-and-down) and horizontal (left-and-right) swing mode. Support must be
   * confirmed by `DeviceCapabilities::hasSwingBoth()`.
   */
  SWING_BOTH = SWING_VERTICAL | SWING_HORIZONTAL,
};

class DeviceCapabilities;

class StatusData : public MideaData {
 public:
  using MideaData::MideaData;
  explicit StatusData()
      : MideaData{0x40, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} {}

  /// Copy status from another StatusData
  void copyStatus(const StatusData &p) { memcpy(m_data.data() + 1, p.m_data.data() + 1, 10); }

  /* TARGET TEMPERATURE */
  float getTargetTemp() const;
  void setTargetTemp(float temp);

  /* MODE */
  OperationMode getRawMode() const { return static_cast<OperationMode>(getValue(2, 7, 5)); }
  OperationMode getMode() const;
  void setMode(OperationMode mode);

  /* FAN SPEED */
  FanSpeed getFanMode() const;
  void setFanMode(FanSpeed mode) { setValue(3, mode); };

  /* SWING MODE */
  SwingMode getSwingMode() const { return static_cast<SwingMode>(getValue(7, 15)); }
  void setSwingMode(SwingMode mode) { setValue(7, 0x30 | mode); }

  /* INDOOR TEMPERATURE */
  float getIndoorTemp() const;

  /* OUTDOOR TEMPERATURE */
  float getOutdoorTemp() const;

  /* HUMIDITY SETPOINT */
  float getHumiditySetpoint() const;

  /* PRESET */
  Preset getPreset() const;
  void setPreset(Preset preset);

  /* POWER USAGE */
  float getPowerUsage() const;

  void setBeeper(bool state) {
    setBit(1, 1, true);
    setBit(1, 6, state);
  }

 protected:
  /* POWER */
  bool m_getPower() const { return getBit(1, 0); }
  void m_setPower(bool state) { setBit(1, 0, state); }
  /* ECO MODE */
  bool m_getEco() const { return getBit(9, 4); }
  void m_setEco(bool state) { setBit(9, 7, state); }
  /* TURBO MODE */
  bool m_getTurbo() const { return getBit(8, 5) || getBit(10, 1); }
  void m_setTurbo(bool state) {
    setBit(8, 5, state);
    setBit(10, 1, state);
  }
  /* FREEZE PROTECTION */
  bool m_getFreezeProtection() const { return getBit(21, 7); }
  void m_setFreezeProtection(bool state) { setBit(21, 7, state); }
  /* SLEEP MODE */
  bool m_getSleep() const { return getBit(10, 0); }
  void m_setSleep(bool state) { setBit(10, 0, state); }
};

enum QueryTypes : uint8_t {
  QUERY_STATE,
  QUERY_POWER_USAGE,
  QUERY_DISPLAY_TOGGLE,
  QUERY_CAPABILITIES,
};

/**
 * @brief PropertiesStateQuery `0xB1`.
 *
 */
class PropertiesStateQuery : public MideaData {
 public:
  // DataBodyQueryB1
  explicit PropertiesStateQuery();
  // DataBodyDevB1
  explicit PropertiesStateQuery(const DeviceCapabilities &s);
};

}  // namespace ac
}  // namespace midea
}  // namespace dongle
