#include "Helpers/IdGenerator.hpp"

namespace helpers {

uint8_t IDGenerator::next() { return ++m_id ? m_id : ++m_id; }

}  // namespace helpers
