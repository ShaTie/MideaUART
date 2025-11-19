#pragma once

#include <initializer_list>

#include "Dongle/Message/MessageData.hpp"

namespace midea {

/**
 * @brief Type of property's 16-bit UUID.
 *
 */
using PropertyUUID = uint16_t;

/**
 * @brief Midea appliance transparent message.
 */
class MideaData : public dongle::MessageData {
  using dongle::MessageData::MessageData;
  friend class PropertiesConsumer;

 public:
  explicit MideaData(std::initializer_list<uint8_t> x) : dongle::MessageData(std::move(x)) { m_finalize(); }

  uint8_t typeID() const { return m_data[IDX_TYPE]; }
  bool assign_status(MideaData &&data);

  /**
   * @brief Checks validity.
   *
   */
  bool isValid() const { return !m_calcChecksum(); }

 protected:
  /**
   * @brief Calculate CRC-8 MAXIM checksum.
   *
   * @return Checksum.
   */
  uint8_t m_calcChecksum() const;

  /**
   * @brief Calculates and updates checksum.
   *
   */
  void m_finalize();

  /**
   * @brief Appends property UUID (for 0xB0, 0xB1 and 0xB5 only).
   *
   * @param uuid UUID of property.
   */
  void appendUUID(PropertyUUID uuid);

  /**
   * @brief Adds a Pascal-type byte array. The array size is added automatically.
   *
   * @param ...data bytes to set.
   */
  template<typename... Args> void appendPascalArray(Args... data) {
    this->append(static_cast<uint8_t>(sizeof...(Args)), static_cast<uint8_t>(data)...);
  }

  /**
   * @brief Append property with specified byte array.
   *
   * @param uuid Property UUID.
   * @param data bytes to set.
   */
  template<typename... Args> void appendProperty(PropertyUUID uuid, Args... data) {
    this->appendUUID(uuid);
    this->appendPascalArray(data...);
  }

  // Constants
  enum : uint8_t {
    IDX_TYPE,
    SET_STATUS = 0x40,
    GET_STATUS = 0x41,
    STATUS_A0 = 0xA0,
    STATUS_A1 = 0xA1,
    SET_PROPERTIES = 0xB0,
    GET_PROPERTIES = 0xB1,
    GET_CAPABILITIES = 0xB5,
    STATUS_C0 = 0xC0,
    POWER_INFO = 0xC1,
  };
};

}  // namespace midea
