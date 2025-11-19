#include <iterator>

#include "Dongle/Message/Message.hpp"

namespace dongle {

Message::Message(MessageTypeID typeID, size_t data_size)
    // Required C++14. Memory allocation (+1 byte for the checksum) filled with zeroes.
    : m_data(std::make_unique<uint8_t[]>(data_size + HEADER_LENGTH + 1)) {
  m_data[IDX_START] = SYM_START;
  m_data[IDX_LENGTH] = data_size + HEADER_LENGTH;
  m_data[IDX_TYPE] = typeID;
}

Message::Message(MessageTypeID typeID, const uint8_t *data, size_t data_size) : Message(typeID, data_size) {
  auto it(&m_data[HEADER_LENGTH]);

  do {
    *it++ = *data++;
  } while (--data_size);
}

uint8_t Message::m_calcChecksum() const {
  auto it(m_data.get());
  uint_fast8_t cs(it[IDX_START]), length(it[IDX_LENGTH]);

  do {
    cs -= *it++;
  } while (--length);

  return cs;
}

void Message::finalize(uint8_t appID, uint8_t protoID) {
  const auto length(m_data[IDX_LENGTH]);

  m_data[IDX_APPLIANCE] = appID;
  m_data[IDX_SYNC] = length ^ appID;
  m_data[IDX_ID] = s_genId.next();
  m_data[IDX_PROTOCOL] = protoID;

  m_data[length] = m_calcChecksum();
}

bool Message::isTransparent() const {
  switch (this->typeID()) {
    case MSG_CONTROL:
    case MSG_QUERY:
    case MSG_NOTIFY_STATUS:
    case MSG_NOTIFY_STATUS_WITH_RESPONSE:
    case MSG_NOTIFY_ERROR:
    case MSG_NOTIFY_ERROR_WITH_RESPONSE:
    case MSG_SET_DATETIME:
    case MSG_GET_INFO:
      return true;
    default:
      return false;
  }
}

}  // namespace dongle
