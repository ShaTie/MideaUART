#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <type_traits>

#include "Dongle/Message/Message.hpp"
#include "Helpers/Utils.hpp"
#include "Helpers/IDGenerator.hpp"

namespace midea {

/**
 * @brief Message types.
 */
enum MideaTypeID : uint8_t {
  /**
   * @brief Device control status data.
   */
  MSG_SET_STATUS = 0x40,

  /**
   * @brief Query device status data. May control LED light in some devices.
   */
  MSG_GET_STATUS = 0x41,

  /**
   * @brief Set device properties.
   */
  MSG_SET_PROPERTIES = 0xB0,

  /**
   * @brief Get device properties.
   */
  MSG_GET_PROPERTIES = 0xB1,

  /**
   * @brief Get device capabilities.
   */
  MSG_GET_CAPABILITIES = 0xB5,

  /**
   * @brief Some status data.
   */
  MSG_STATUS_A0 = 0xA0,

  /**
   * @brief Some status data.
   */
  MSG_STATUS_A1 = 0xA1,

  /**
   * @brief Main status data.
   */
  MSG_STATUS_C0 = 0xC0,

  /**
   * @brief Real-time power consumption data in BCD format.
   */
  MSG_STATUS_C1 = 0xC1,
};

/**
 * @brief Message class.
 *
 */
class MideaMessage : public Message {
 public:
  MideaMessage(MideaTypeID typeID, size_t data_size) : Message(s_type(typeID), data_size + 2) { m_at(0) = typeID; }
  static MessageTypeID s_type(MideaTypeID mideaID) {
    switch (mideaID) {
      case MSG_SET_STATUS:
      case MSG_SET_PROPERTIES:
        return MSG_CONTROL;
      default:
        return MSG_QUERY;
    }
  }
};

}  // namespace midea
