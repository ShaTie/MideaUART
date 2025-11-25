#include <algorithm>

#include "Midea/AirConditioner/DeviceCapabilities.hpp"
#include "Midea/Message/PropertiesConsumer.hpp"
#include "Helpers/Helpers.hpp"
#include "Helpers/Logger.hpp"

namespace midea {
namespace ac {

static const char *const TAG = "DeviceCapabilities";

bool DeviceCapabilities::isQueryNeeded() const {
  return m_hasSelfClean || m_hasNoWindFeel || m_hasOneKeyNoWindOnMe || m_hasBreeze || m_hasBuzzer || m_hasSmartEye ||
         m_hasIndoorHumidity || m_hasVerticalWind || m_hasHorizontalWind || m_isTwins || m_isFourDirection;
}

static auto prvMode(uint_fast8_t x) {
  switch (x) {
    case 1:
      return 0b1111;  // DRY | HEAT | AUTO | COOL
    case 2:
      return 0b0110;  // HEAT | AUTO
    case 3:
      return 0b0001;  // COOL
    case 4:
      return 0b0101;  // HEAT | COOL
    case 5:
      return 0b1001;  // DRY | COOL
    default:
      return 0b1011;  // DRY | AUTO | COOL
  }
}

static auto prvSwing(uint_fast8_t x) {
  switch (x) {
    case 1:
      return 0b11;  // HORIZONTAL | VERTICAL
    case 2:
      return 0b00;  //
    case 3:
      return 0b10;  // HORIZONTAL
    default:
      return 0b01;  // VERTICAL
  }
}

static auto prvFanSpeed(uint_fast8_t x) {
  switch (x) {
    case 1:
      return 0b11111;  // RAW | AUTO | HIGH | MEDIUM | LOW
    case 2:
      return 0b00001;  // LOW
    case 3:
      return 0b00101;  // HIGH | LOW
    case 4:
      return 0b01101;  // AUTO | HIGH | LOW
    case 7:
      return 0b00111;  // HIGH | MEDIUM | LOW
    default:
      return 0b01111;  // AUTO | HIGH | MEDIUM | LOW
  }
}

static auto prvDrySmart(uint_fast8_t x) {
  switch (x) {
    case 1:
      return 0b01;  // AUTO
    case 2:
      return 0b11;  // CUSTOM | AUTO
    case 3:
      return 0b10;  // CUSTOM
    default:
      return 0b00;  //
  }
}

static auto prvTurbo(uint_fast8_t x) {
  switch (x) {
    case 0:
      return 0b01;  // COOL
    case 2:
      return 0b00;  //
    case 3:
      return 0b10;  // HEAT
    default:
      return 0b11;  // HEAT | COOL
  }
}

static auto prvPower(uint_fast8_t x) {
  switch (x) {
    case 2:
      return 0b01;
    case 3:
      return 0b11;
    default:
      return 0b00;
  }
}

static auto prvFilter(uint_fast8_t x) {
  switch (x) {
    case 0:
      return 0b00;
    case 3:
      return 0b10;
    case 4:
      return 0b11;
    default:
      return 0b01;
  }
}

static auto prvEco(uint_fast8_t x) {
  switch (x) {
    case 1:
      return 0b01;
    case 2:
      return 0b11;
    default:
      return 0b00;
  }
}

auto DeviceCapabilities::hasMode(OperationMode x) const -> bool {
  switch (x) {
    case MODE_AUTO:
      return hasModeAuto();
    case MODE_COOL:
      return hasModeCool();
    case MODE_DRY:
      return hasModeDry();
    case MODE_HEAT:
      return hasModeHeat();
    case MODE_FAN:
      return true;
    case MODE_DRY_CUSTOM:
      return hasModeDryCustom();
    default:
      return false;
  }
}

auto DeviceCapabilities::tempRangeMax() const -> TempRange {
  return {
      std::min({m_tempCool.min, m_tempAuto.min, m_tempHeat.min}),
      std::max({m_tempCool.max, m_tempAuto.max, m_tempHeat.max}),
  };
}

auto DeviceCapabilities::tempRange(OperationMode mode) const -> const TempRange & {
  switch (mode) {
    case MODE_AUTO:
      return m_tempAuto;
    case MODE_HEAT:
      return m_tempHeat;
    default:
      return m_tempCool;
  }
}

auto DeviceCapabilities::hasFanSpeed(uint_fast8_t x) const -> bool {
  switch (x) {
    case FAN_AUTO:
      return hasFanAuto();
    case FAN_LOW:
      return hasFanLow();
    case FAN_MEDIUM:
      return hasFanMedium();
    case FAN_HIGH:
      return hasFanHigh();
    default:
      return hasFanRaw() && (x <= 100);
  }
}

inline auto DeviceCapabilities::m_temp(const uint8_t *x) -> void {
  m_tempCool = TempRange(x[0], x[1]);
  m_tempAuto = TempRange(x[2], x[3]);
  m_tempHeat = TempRange(x[4], x[5]);
  m_isHavePoint = bool(x[6]);
}

auto DeviceCapabilities::m_onProperty(const Property &x) -> void {
  const uint_fast8_t data(*x.data());
  const uint_fast16_t uuid(x.uuid());
  const bool nzero(data);

  switch (uuid) {
    case CapabilityUUID::UUID_TEMP:
      m_temp(x.data());
      break;
    case CapabilityUUID::UUID_MODES:
      m_hasMode = prvMode(data);
      break;
    case CapabilityUUID::UUID_SWING:
      m_hasSwing = prvSwing(data);
      break;
    case CapabilityUUID::UUID_FAN:
      m_hasFan = prvFanSpeed(data);
      break;
    case CapabilityUUID::UUID_DRY:
      m_hasDrySmart = prvDrySmart(data);
      break;
    case CapabilityUUID::UUID_TURBO:
      m_hasTurbo = prvTurbo(data);
      break;
    case CapabilityUUID::UUID_ECO:
      m_hasEco = prvEco(data);
      break;
    case CapabilityUUID::UUID_POWER:
      m_hasPower = prvPower(data);
      break;
    case CapabilityUUID::UUID_FILTER:
      m_hasFilter = prvFilter(data);
      break;
    case CapabilityUUID::UUID_FAHRENHEIT:
      m_unitChangeable = !nzero;  // not mistake. 0 is true.
      break;
    case CapabilityUUID::UUID_VWIND:
      m_hasVerticalWind = nzero;
      break;
    case CapabilityUUID::UUID_HWIND:
      m_hasHorizontalWind = nzero;
      break;
    case CapabilityUUID::UUID_HUMIDITY:
      m_hasIndoorHumidity = nzero;
      break;
    case CapabilityUUID::UUID_SILKY_COOL:
      m_hasNoWindFeel = nzero;
      break;
    case CapabilityUUID::UUID_ECO_EYE:
      m_hasSmartEye = nzero;
      break;
    case CapabilityUUID::UUID_SELF_CLEAN:
      m_hasSelfClean = nzero;
      break;
    case CapabilityUUID::UUID_WIND_ON_ME:
      m_hasBlowingPeople = nzero;
      break;
    case CapabilityUUID::UUID_WIND_OFF_ME:
      m_hasAvoidPeople = nzero;
      break;
    case CapabilityUUID::UUID_BREEZE_AWAY:
      m_hasOneKeyNoWindOnMe = nzero;
      break;
    case CapabilityUUID::UUID_BREEZELESS:
      m_hasBreeze = nzero;
      break;
    case CapabilityUUID::UUID_BUZZER:
      m_hasBuzzer = nzero;
      break;
    case CapabilityUUID::UUID_AUX_HEATER:
      m_dianfure = nzero;
      break;
    case CapabilityUUID::UUID_LIGHT:
      m_hasLight = nzero;
      break;
    case CapabilityUUID::UUID_TWINS:
      m_isTwins = nzero;
      break;
    case CapabilityUUID::UUID_8HEAT:
      m_eightHot = nzero;
      break;
    case CapabilityUUID::UUID_FOUR_DIRECTION:
      m_isFourDirection = nzero;
      break;
    default:
      LOG_D(TAG, "Unknown capability 0x%04X", uuid);
      break;
  }
}

auto DeviceCapabilities::dump() const -> void {
#if LOG_LEVEL >= LOG_LEVEL_CONFIG
#define LOG_CAPABILITY(str, condition) \
  if (condition) \
    LOG_CONFIG(TAG, str);

  LOG_CONFIG(TAG, "Midea Capabilities Report:");

  if (hasModeAuto()) {
    LOG_CONFIG(TAG, "  [x] Auto Mode");
    LOG_CONFIG(TAG, "      - Min: %d", m_tempAuto.min / 2);
    LOG_CONFIG(TAG, "      - Max: %d", m_tempAuto.max / 2);
  }

  if (hasModeCool()) {
    LOG_CONFIG(TAG, "  [x] Cool Mode");
    LOG_CONFIG(TAG, "      - Min: %d", m_tempCool.min / 2);
    LOG_CONFIG(TAG, "      - Max: %d", m_tempCool.max / 2);
    LOG_CAPABILITY("      - Turbo", hasTurboCool());
  }

  if (hasModeHeat()) {
    LOG_CONFIG(TAG, "  [x] Heat Mode");
    LOG_CONFIG(TAG, "      - Min: %d", m_tempHeat.min / 2);
    LOG_CONFIG(TAG, "      - Max: %d", m_tempHeat.max / 2);
    LOG_CAPABILITY("      - Turbo", hasTurboHeat());
    // Only in `HEAT` mode and not supported by `SLEEP`.
    LOG_CAPABILITY("      - 8°C Frost Protection", m_eightHot);
  }

  if (hasModeDry()) {
    LOG_CONFIG(TAG, "  [x] Dry Mode");
    //
    LOG_CAPABILITY("      - Smart", hasDrySmart());
    // MODE_DRY_CUSTOM supported. .humidity in command is setpoint
    LOG_CAPABILITY("      - Custom", hasModeDryCustom());
  }

  LOG_CONFIG(TAG, "  [x] Fan Speeds:");
  LOG_CAPABILITY("      - RAW", hasFanRaw());
  LOG_CAPABILITY("      - AUTO", hasFanAuto());
  LOG_CAPABILITY("      - LOW", hasFanLow());
  LOG_CAPABILITY("      - MEDIUM", hasFanMedium());
  LOG_CAPABILITY("      - HIGH", hasFanHigh());
  LOG_CAPABILITY("  [x] Indoor Humidity", m_hasIndoorHumidity);  // Indoor humidity in B1 response
  LOG_CAPABILITY("  [x] Decimal Point", m_isHavePoint);
  LOG_CAPABILITY("  [x] Fahrenheits", m_unitChangeable);
  LOG_CAPABILITY("  [x] Breezeless", m_hasBreeze);  // Only in `COOL` mode. Valid values are: 1, 2, 3, 4.
  LOG_CAPABILITY("  [x] Vertical Direction", m_hasVerticalWind);
  LOG_CAPABILITY("  [x] Horizontal Direction", m_hasHorizontalWind);
  LOG_CAPABILITY("  [x] Twins", m_isTwins);
  LOG_CAPABILITY("  [x] Four Direction", m_isFourDirection);
  LOG_CAPABILITY("  [x] Vertical Swing", hasSwingVertical());
  LOG_CAPABILITY("  [x] Horizontal Swing", hasSwingHorizontal());
  LOG_CAPABILITY("  [x] Silky Cool", m_hasNoWindFeel);         // Only in `COOL` mode. Temperature: >= 24°C.
  LOG_CAPABILITY("  [x] ECO", hasEco());                       // Only in `COOL` mode. Temperature: >= 24°C.
  LOG_CAPABILITY("  [x] Special ECO", hasEcoSpecial());        // Only in `AUTO`, `COOL`, `DRY` modes.
  LOG_CAPABILITY("  [x] ECO Intelligent Eye", m_hasSmartEye);  // Not supported modes: `DRY`, `FAN_ONLY`.
  // in HEAT mode DeviceStatus.ptcAssis must be 1
  LOG_CAPABILITY("  [x] Electric Auxiliary Heat", m_dianfure);
  LOG_CAPABILITY("  [x] Wind ON me", m_hasBlowingPeople);
  LOG_CAPABILITY("  [x] Wind OFF me", m_hasAvoidPeople);
  LOG_CAPABILITY("  [x] LED", m_hasLight);
  LOG_CAPABILITY("  [x] Buzzer", m_hasBuzzer);
  LOG_CAPABILITY("  [x] Active Clean", m_hasSelfClean);
  LOG_CAPABILITY("  [x] Breeze Away", m_hasOneKeyNoWindOnMe);
  // DeviceStatus.dusFull. cleanFanTime == 1 in command clear timer
  LOG_CAPABILITY("  [x] Air Filter Cleaning Reminder", hasFilterCleanReminder());
  LOG_CAPABILITY("  [x] Air Filter Replacement Reminder", hasFilterReplaceReminder());
  LOG_CAPABILITY("  [x] Power Report", hasPowerReport());
  LOG_CAPABILITY("  [x] Power Limit", hasPowerLimits());
#endif
}

}  // namespace ac
}  // namespace midea
