#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <utility>
#include <string>
#include <cstring>
#include "Helpers/IdGenerator.hpp"
#include "Helpers/Utils.hpp"
#include "Dongle/Message/MessageHeader.hpp"

namespace dongle {

enum MessageTypeID : uint8_t {
  MSG_CONTROL = 0x02,
  MSG_QUERY = 0x03,
  MSG_NOTIFY_STATUS = 0x04,
  MSG_NOTIFY_STATUS_WITH_RESPONSE = 0x05,
  MSG_NOTIFY_ERROR = 0x06,
  MSG_GET_ELECTRONIC_ID = 0x07,
  MSG_NOTIFY_ERROR_WITH_RESPONSE = 0x0A,
  MSG_NOTIFY_NETWORK_STATUS = 0x0D,
  MSG_SET_ELECTRONIC_ID = 0x11,
  MSG_SET_SSID = 0x12,
  MSG_GET_MAC = 0x13,
  MSG_SET_DATETIME = 0x61,
  MSG_GET_NETWORK_STATUS = 0x63,
  MSG_SET_WIFI_STATE = 0x68,
  MSG_SETUP_WIFI_CLIENT = 0x6A,
  MSG_GET_AP_LIST = 0x6B,
  MSG_SET_WIFI_MODE = 0x81,
  MSG_RESET_SOFT = 0x82,
  MSG_RESET_HARD = 0x83,
  MSG_GET_INFO = 0xA0,
};

enum MideaType : uint8_t {
  SET_STATUS = 0x40,
  GET_STATUS = 0x41,
  SET_PROPERTIES = 0xB0,
  GET_PROPERTIES = 0xB1,
  GET_CAPABILITIES = 0xB5,
};

template<size_t N> using array_t = std::array<uint8_t, N>;

template<typename T> class StaticFrame : public MessageHeader {
  static constexpr size_t LEN = sizeof(MessageHeader) + sizeof(T);

 public:
  /**
   * @brief Constructs message with template data from bytes initializer list.
   * @param appID appliance type identifier.
   * @param frameID message identifier.
   * @param protoID appliance protocol version identifier.
   * @param typeID message type identifier.
   */
  StaticFrame(uint8_t appID, uint8_t frameID, uint8_t protoID, uint8_t typeID)
      : MessageHeader(LEN, appID, frameID, protoID, typeID) {}

  /**
   * @brief Constructs message with template data from bytes initializer list.
   * @param appID appliance type identifier.
   * @param frameID message identifier.
   * @param protoID appliance protocol version identifier.
   * @param typeID message type identifier.
   * @param args message data initializer list.
   */
  StaticFrame(uint8_t appID, uint8_t frameID, uint8_t protoID, uint8_t typeID, std::initializer_list<uint8_t> args)
      : MessageHeader(LEN, appID, frameID, protoID, typeID), m_data(args) {}

  /**
   * @brief Constructs message with template data.
   * @tparam ...Args types of message data initialization arguments.
   * @param appID appliance type identifier.
   * @param frameID message identifier.
   * @param protoID appliance protocol version identifier.
   * @param typeID message type identifier.
   * @param ...args message data initialization arguments.
   */
  template<typename... Args>
  StaticFrame(uint8_t appID, uint8_t frameID, uint8_t protoID, uint8_t typeID, Args... args)
      : MessageHeader(LEN, appID, frameID, protoID, typeID), m_data(args...) {}

  /**
   * @brief Returns reference to message data.
   * @return Reference to message data.
   */
  T &data() { return m_data; }

  /**
   * @brief Returns constant reference to message data.
   * @return Constant reference to message data.
   */
  const T &data() const { return m_data; }

 protected:
  T m_data;  // Frame data. To avoid alignments, it must consist of byte fields only.

 private:
  uint8_t m_reserved4;  // reserved for message checksum
};

template<typename T> class MideaData : public T {
  uint8_t m_checksum;
  uint8_t m_calcChecksum() const {
    return helpers::Utils::crc8maxim(reinterpret_cast<const uint8_t *>(this), &m_checksum);
  }

 public:
  bool check() const { return m_calcChecksum() == m_checksum; }
  void finalize() { m_checksum = m_calcChecksum(); }
};

}  // namespace dongle
