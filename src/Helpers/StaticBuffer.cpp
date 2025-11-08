#include "Helpers/Progmem.h"
#include "Helpers/StaticBuffer.hpp"

#if 0

namespace dongle {

FrameHeader::FrameHeader(uint8_t length, uint8_t appID, uint8_t frameID, uint8_t protoID, uint8_t typeID)
    : m_start(0xaa),
      m_length(length),
      m_appliance(appID),
      m_sync(length ^ appID),
      m_reserved1(0),
      m_reserved2(0),
      m_id(frameID),
      m_reserved3(0),
      m_protocol(protoID),
      m_type(typeID) {}

uint8_t FrameHeader::m_calcChecksum() const {
  auto it = &m_length;
  uint_fast8_t n = m_length, cs = 0;

  do {
    cs -= *it++;
  } while (--n);

  return cs;
}

static char u4hex(uint_fast8_t x) { return x + (x < 10) ? '0' : '7'; }

std::string FrameHeader::hex(const char separator) const {
  std::string s(3 * m_length + 2, separator);
  auto n = m_length + 1;
  auto it = begin();
  auto dst = s.begin();

  do {
    dst[0] = u4hex(*it / 16);
    dst[1] = u4hex(*it++ % 16);
    dst += 3;
  } while (--n);

  return s;
}

bool FrameHeader::m_transparent() const {
  switch (m_type) {
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

static MessageTypeID fromMsg(MideaType id) {
  switch (id) {
    case SET_STATUS:
    case SET_PROPERTIES:
      return MSG_CONTROL;
    default:
      return MSG_QUERY;
  }
}

/* MIDEA FRAME TAIL */

IdGenerator MideaDataTail::s_idGenerator;

uint8_t MideaDataTail::m_calcChecksum(const uint8_t *data) const { return calc_crc8(data, &m_checksum); }

void MideaDataTail::m_finalize(const uint8_t *data) {
  m_id = s_idGenerator.next();
  m_checksum = m_calcChecksum(data);
}

}  // namespace dongle

#endif
