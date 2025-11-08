#pragma once

#include <array>

#include "Dongle/Message/Buffer.hpp"

namespace helpers {
class WifiNotifierBase;
}

namespace dongle {

class MessageData : public Buffer {
  using Buffer::Buffer;

 public:
  static std::array<uint8_t, 20> networkNotify(const helpers::WifiNotifierBase &x);
  // static MessageData networkNotify(const helpers::WifiNotifierBase &);
  static MessageData getElectronicIdQuery() { return MessageData(1); }
};

}  // namespace dongle
