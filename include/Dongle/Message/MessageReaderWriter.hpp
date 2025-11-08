#pragma once

#include "Dongle/Message/Message.hpp"
#include "Helpers/IDGenerator.hpp"

namespace dongle {

class ITransport;

constexpr size_t RX_BUFFER_SIZE = 64;

/**
 * @brief Message reader class.
 *
 */
class MessageReaderWriter : public Message {
 public:
  /**
   * @brief Constructor.
   * @param io I/O interface.
   */
  explicit MessageReaderWriter(size_t buffer_size, ITransport *io = nullptr) : Message(buffer_size), m_io(io) {}

  /**
   * @brief Sets stream interface instance.
   * @param io I/O interface.
   */
  void setTransport(ITransport *io) { m_io = io; }

  /**
   * @brief Clears message after handling.
   */
  void clear() { m_end = m_data.get(); }

  /**
   * @brief Deserializes message byte by byte. Caller is responsible for clearing message after handling.
   * @return `true` if message deserializing is complete and it ready for handling.
   */
  bool read();

  /**
   * @brief Creates message instance from parameters and writes it to the stream.
   * @param typeID type of message.
   * @param s message data body.
   */
  void write(Message &s);

 protected:
  // End pointer
  uint8_t *m_end;

  // I/O stream interface
  ITransport *m_io{nullptr};

  // Message ID generator
  helpers::IDGenerator m_id;

  // Appliance ID
  ApplianceID m_appID{APP_BROADCAST};

  // Protocol ID
  uint8_t m_protoID{};

 private:
  // Updates appliance ID
  void m_updAppID();

  // Updates protocol ID
  void m_updProtoID();
};

}  // namespace dongle
