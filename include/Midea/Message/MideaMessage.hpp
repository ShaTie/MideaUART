#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <type_traits>

#include "Dongle/Message/Message.hpp"
#include "Helpers/Utils.hpp"
#include "Helpers/IDGenerator.hpp"

namespace dongle {
namespace midea {

/**
 * @brief Message class.
 *
 */
class MideaMessage : public Message {
 public:
  MideaMessage(uint8_t typeID, size_t data_size) : Message(s_type(typeID), data_size + 2) { m_at(0) = typeID; }
  static MessageTypeID s_type(uint8_t mideaID);
};

}  // namespace midea
}  // namespace dongle
