#include "Dongle/Message/MessageReaderWriter.hpp"
#include "Dongle/Transport/ITransport.hpp"
#include "Helpers/Logger.hpp"
#include "Helpers/IDGenerator.hpp"

namespace dongle {

using helpers::IDGenerator;

static const char *const TAG = "MessageReaderWriter";

inline void MessageReaderWriter::m_updAppID() {
  auto appID = ApplianceID(m_data[IDX_APPLIANCE]);

  if (appID == m_appID)
    return;

  m_appID = appID;

  LOG_D(TAG, "ApplianceID updated to 0x%02X", appID);
}

inline void MessageReaderWriter::m_updProtoID() {
  auto protoID(m_data[IDX_PROTOCOL]);

  if (protoID == m_protoID)
    return;

  m_protoID = protoID;

  LOG_D(TAG, "ProtocolID updated to %d", protoID);
}

bool MessageReaderWriter::read() {
  for (char x; m_io->read(x);) {
    const auto idx = std::distance(m_data.get(), m_end);
    const uint8_t data(x);
    *m_end++ = data;

    switch (idx) {
      case IDX_START:
        if (data == SYM_START)
          continue;
        break;

      case IDX_LENGTH:
        if (data > HEADER_LENGTH)
          continue;
        break;

      default:
        if (idx < m_data[IDX_LENGTH])
          continue;

        if (m_calcChecksum() != data) {
          LOG_W(TAG, "Checksum is wrong.");
          break;
        }

        LOG_D(TAG, "RX: %s", this->hex().c_str());

        m_updAppID();
        m_updProtoID();

        return true;
    }

    clear();
  }

  return false;
}

void MessageReaderWriter::write(Message &msg) {
  msg.finalize(m_appID, m_protoID);
  LOG_D(TAG, "TX: %s", msg.hex().c_str());
  m_io->write(reinterpret_cast<char *>(msg.m_data.get()), msg.m_size());
}

}  // namespace dongle
