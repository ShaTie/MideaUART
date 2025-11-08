#pragma once

#include <cstdint>
#include <iterator>
#include <string>

namespace helpers {

class Utils {
 public:
  static uint8_t crc8maxim(const uint8_t *begin, const uint8_t *end);
  static uint8_t crc8maxim(const uint8_t *data, size_t size) { return crc8maxim(data, data + size); }
  static std::string hex(const uint8_t *data, size_t size);
  static std::string hex(const uint8_t *begin, const uint8_t *end) { return hex(begin, std::distance(begin, end)); }
};

}  // namespace helpers
