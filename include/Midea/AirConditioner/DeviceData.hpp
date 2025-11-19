#pragma once

#include "Midea/Message/MideaData.hpp"

namespace midea {
namespace ac {

class DeviceCapabilities;
class DeviceControl;

class DeviceData : public MideaData {
  using MideaData::MideaData;

 public:
  /**
   * @brief Creates power status query.
   * @return Returns power status query data.
   */
  static DeviceData getPowerQuery();

  /**
   * @brief Creates status query.
   * @return Returns status query data.
   */
  static DeviceData getStatusQuery();

  /**
   * @brief Creates capabilities query for specified `n`.
   * @param n capabilities page number.
   * @return Returns capabilities query data.
   */
  static DeviceData getCapabilitiesQuery(uint8_t n = 0);

  /**
   * @brief Creates light toggle query.
   * @return Returns light toggle query data.
   */
  static DeviceData lightToggleQuery();

  /**
   * @brief Creates set status command from `ReadableStatusOld`.
   * @param x `DeviceControl` instance.
   * @return Returns set status command data.
   */
  static DeviceData setStatusQuery(const DeviceControl &x);

  static DeviceData getPropertiesQuery(const DeviceCapabilities &x);
};

}  // namespace ac
}  // namespace midea
