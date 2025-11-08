#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <type_traits>

#include "Dongle/Message/Buffer.hpp"
#include "Dongle/Message/MessageData.hpp"
#include "Helpers/Utils.hpp"
#include "Helpers/IDGenerator.hpp"

namespace dongle {

/**
 * @brief Appliance types ID.
 *
 */
enum ApplianceID : uint8_t {
  APP_DEHUMIDIFIER = 0xA1,    /**< Dehumidifier */
  APP_AIR_CONDITIONER = 0xAC, /**< Air conditioner */
  APP_HUMIDIFIER = 0xFD,      /**< Humidifier */
  APP_BROADCAST = 0xFF,       /**< Broadcast */
};

/**
 * @brief Message types.
 */
enum MessageTypeID : uint8_t {
  /**
   * @brief Device Control Command.
   *
   * Used for transparent transmission of data from the network end to the home appliance end to control the
   * actions of the home appliance.
   */
  MSG_CONTROL = 0x02,
  /**
   * @brief Device Query Command.
   *
   * Used for transparent data transmission from the network end to the home appliance end and querying the
   * status of home appliances.
   */
  MSG_QUERY = 0x03,
  /**
   * @brief Device operating parameter reporting (no response).
   *
   * Used for transparent transmission of data from the appliance to the network (no response required). Module does not
   * respond to this type of message. Analyze the content.
   */
  MSG_NOTIFY_STATUS = 0x04,
  /**
   * @brief Device operating parameter reporting (response required).
   *
   * Used for transparent data transmission from the home appliance to the network (requiring a response). Module does
   * not parse the content of this type of message. After receiving the response from the network, module returns a
   * response to the home appliance.
   */
  MSG_NOTIFY_STATUS_WITH_RESPONSE = 0x05,
  /**
   * @brief Device abnormal event reporting (no response).
   *
   * Used for transparent transmission of data from the appliance to the network (no response required). Module does not
   * respond to this type of message. Analyze the content.
   */
  MSG_NOTIFY_ERROR = 0x06,
  /**
   * @brief Device Electronic ID Acquisition.
   *
   * ID information from the appliance during module initialization. For communication security reasons, this
   * message can only be initiated by module to the appliance and must not be sent from the network to the appliance via
   * data transparent transmission.
   */
  MSG_GET_ELECTRONIC_ID = 0x07,
  /**
   * @brief Device abnormal event reporting (response required).
   *
   * Used for transparent data transmission from the home appliance to the network (requiring a response). Module does
   * not parse the content of this type of message. After receiving the response from the network, module returns a
   * response to the home appliance.
   */
  MSG_NOTIFY_ERROR_WITH_RESPONSE = 0x0A,
  /**
   * @brief Device networking notification.
   *
   * Whenever module's networking status (as shown in the table below) changes, the home appliance is notified of the
   * latest networking status.
   */
  MSG_NOTIFY_NETWORK_STATUS = 0x0D,
  /**
   * @brief Device Electronic ID Write.
   */
  MSG_SET_ELECTRONIC_ID = 0x11,
  /**
   * @brief SSID Rename.
   *
   * Only in AP After receiving the message, module will send the network hotspot SSID Change the name to the one
   * specified in the message and clear the stored SSID and password information.
   */
  MSG_SET_SSID = 0x12,
  /**
   * @brief Read MAC address.
   */
  MSG_GET_MAC = 0x13,
  /**
   * @brief Time calibration.
   */
  MSG_SET_DATETIME = 0x61,
  /**
   * @brief Home appliance queries network and signal status.
   */
  MSG_GET_NETWORK_STATUS = 0x63,
  /**
   * @brief Wi-Fi signal on/off command.
   *
   * After receiving this command, module turns Wi-Fi on or off according to the message content. RF signal transmission
   * and reception.
   */
  MSG_SET_WIFI_STATE = 0x68,
  /**
   * @brief Wi-Fi Parameter Configuration.
   *
   * In AP In this mode, you can use the input device of the home appliance to input the SSID of the target router to
   * which module needs to connect and password to Wi-Fi Module.
   */
  MSG_SETUP_WIFI_CLIENT = 0x6A,
  /**
   * @brief Home appliance query AP list.
   *
   * Home appliances move to Wi-Fi Module requests the AP discovered by module After receiving the command, module scans
   * the nearby APs and records the top 20 APs according to signal strength. APs SSID and encryption method, and then
   * returns a response to the home appliance. The response message body length does not exceed 245 bytes.
   */
  MSG_GET_AP_LIST = 0x6B,
  /**
   * @brief Wi-Fi Working mode switch.
   *
   * This message is used to control Wi-Fi Module in AP and STA mode.
   */
  MSG_SET_WIFI_MODE = 0x81,
  /**
   * @brief Restart Wi-Fi module.
   */
  MSG_RESET_SOFT = 0x82,
  /**
   * @brief Wi-Fi module factory reset.
   */
  MSG_RESET_HARD = 0x83,
  /**
   * @brief Home appliance model and basic information query.
   */
  MSG_GET_INFO = 0xA0,
};

/**
 * @brief Message class.
 *
 */
class Message {
  friend class MessageReaderWriter;

 public:
  /**
   * @brief Creates a `Message` instance of the specified type and data size. The data is filled with zeros.
   *
   * @param type Type ID.
   * @param data_size Data size.
   *
   * @return `Message` instance.
   */
  explicit Message(MessageTypeID type, size_t data_size);

  /**
   * @brief Makes `Message` instance from parameters.
   *
   * @param type Type ID.
   * @param data Data.
   * @param data_size Data size.
   *
   * @return `Message` instance.
   */
  explicit Message(MessageTypeID type, const uint8_t *data, size_t data_size);

  /**
   * @brief Makes `Message` instance from parameters.
   *
   * @param type Type ID.
   * @param data Initialize list with data.
   *
   * @return `Message` instance.
   */
  explicit Message(MessageTypeID type, std::initializer_list<uint8_t> data)
      : Message(type, data.begin(), data.size()) {}

  /**
   * @brief Makes `Message` instance from parameters.
   *
   * @param type Type ID.
   * @param data Templated container with data. Example : `std::array` or `std::vector`.
   *
   * @return `Message` instance.
   */
  template<typename T, typename std::enable_if<!std::is_integral<T>::value>::type>
  explicit Message(MessageTypeID type, const T &data) : Message(type, data.data(), data.size()) {}

  /**
   * @brief Finalize message and prepare for transmit.
   *
   * @param appID Appliance ID.
   * @param protoID Appliance protocol ID.
   */
  void finalize(ApplianceID appID, uint8_t protoID);

  /**
   * @brief Access for element by index.
   *
   * @param index index of element.
   * @return uint8_t& element.
   */
  uint8_t &operator[](size_t index) { return m_at(index); }

  /**
   * @brief Access for const element by index.
   *
   * @param index index of element.
   * @return const uint8_t& element.
   */
  const uint8_t &operator[](size_t index) const { return m_at(index); }

  /**
   * @brief Returns pointer to start of message data.
   * @return Pointer to start of message data.
   */
  uint8_t *data_begin() { return &m_data[HEADER_LENGTH]; }

  /**
   * @brief Returns constant pointer to start of message data.
   * @return Constant pointer to start of message data.
   */
  const uint8_t *data_begin() const { return &m_data[HEADER_LENGTH]; }

  /**
   * @brief Returns pointer to end of message data.
   * @return Pointer to end of message data.
   */
  uint8_t *data_end() { return &m_data[m_data[IDX_LENGTH]]; }

  /**
   * @brief Returns constant pointer to end of message data.
   * @return Constant pointer to end of message data.
   */
  const uint8_t *data_end() const { return &m_data[m_data[IDX_LENGTH]]; }

  /**
   * @brief Returns size of message data.
   * @return Size of message data.
   */
  size_t data_size() const { return m_data[IDX_LENGTH] - HEADER_LENGTH; }

  /**
   * @brief Check if message is transparent and using manufacturer protocol data.
   *
   * @return Message transparency.
   */
  bool isTransparent() const;

  /**
   * @brief Gets message type.
   *
   * @return message type.
   */
  MessageTypeID type() const { return MessageTypeID(m_data[IDX_TYPE]); }

  /**
   * @brief Check message type.
   *
   * @param value message type.
   * @return `true` if message has specified type.
   */
  bool hasType(MessageTypeID type) const { return this->type() == type; }

  /**
   * @brief Message ID.
   *
   * @return message ID.
   */
  uint8_t id() const { return m_data[IDX_ID]; }

  /**
   * @brief Protocol ID.
   *
   * @return protocol ID.
   */
  uint8_t protocolID() const { return m_data[IDX_PROTOCOL]; }

  /**
   * @brief Print message as hex string.
   *
   * @return Hex string.
   */
  std::string hex() const { return helpers::Utils::hex(m_data.get(), m_size()); }

  template<typename T> T &ref(size_t index = 0) { return reinterpret_cast<T &>(m_at(index)); }
  template<typename T> const T &ref(size_t index = 0) const { return reinterpret_cast<const T &>(m_at(index)); }
  template<typename T> T *ptr(size_t index = 0) { return reinterpret_cast<T *>(&m_at(index)); }
  template<typename T> const T *ptr(size_t index = 0) const { return reinterpret_cast<const T *>(&m_at(index)); }

 protected:
  /// Allocated memory.
  std::unique_ptr<uint8_t[]> m_data;
  ///
  static helpers::IDGenerator s_genId;

  /**
   * @brief Protected default constructor. Used for `MessageReaderWriter` instance construction.
   *
   */
  explicit Message(size_t buffer_size) : m_data(new uint8_t[buffer_size]) {}

  /**
   * @brief Access for element by index.
   *
   * @param index index of element.
   * @return uint8_t& element.
   */
  uint8_t &m_at(size_t index) { return data_begin()[index]; }

  /**
   * @brief Access for const element by index.
   *
   * @param index index of element.
   * @return const uint8_t& element.
   */
  const uint8_t &m_at(size_t index) const { return data_begin()[index]; }

  // Calculates checksum.
  uint8_t m_calcChecksum() const;

  // Full message size.
  size_t m_size() const { return m_data[IDX_LENGTH] + 1; }

  // Constants
  enum : uint8_t {
    IDX_START,        /**< Index of synchronization header */
    IDX_LENGTH,       /**< Index of message length */
    IDX_APPLIANCE,    /**< Index of home appliance type */
    IDX_SYNC,         /**< Index of message synchronization check */
    IDX_ID = 6,       /**< Index of message identifier */
    IDX_PROTOCOL = 8, /**< Index of appliance protocol version */
    IDX_TYPE,         /**< Index of message type identifier */
    HEADER_LENGTH,    /**< Header length */
    SYM_START = 0xAA, /**< Synchronization header */
  };
};

}  // namespace dongle
