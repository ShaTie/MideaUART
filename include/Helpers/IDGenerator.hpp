#pragma once

#include <cstdint>

namespace helpers {

class IDGenerator {
 public:
  uint8_t get() const { return m_id; }
  uint8_t next() { return ++m_id ? m_id : ++m_id; }

 private:
  uint8_t m_id{};
};

}  // namespace helpers
