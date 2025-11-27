#pragma once

#include "Midea/Message/MideaData.hpp"

namespace midea {

class Property {
 public:
  /**
   * @brief Property UUID.
   * @return UUID.
   */
  PropertyUUID uuid() const { return m_header[1] * 256 + m_header[0]; }

  /**
   * @brief Returns data pointer.
   * @return Pointer to data.
   */
  const uint8_t *data() const { return m_data; }

  /**
   * @brief
   * @return
   */
  bool valid() const { return std::distance(m_data, m_end) >= size(); }

  /**
   * @brief Size of properties data.
   * @return size of properties data.
   */
  unsigned size() const { return m_data[-1]; }

  /**
   * @brief Result of operation. Valid only for `0xB0` and `0xB1` messages.
   * @return Result of operation.
   */
  unsigned result() const { return m_data[-2]; }

 protected:
  Property(const MideaData &x) : m_header(&x[2]), m_data(&x[5]), m_end(&x.buffer().back()) {
    if (x[0] != 0xB5)
      m_data++;
  }

  const uint8_t *m_header, *m_data, *const m_end;
};

/**
 * @brief Properties consumer base class.
 */
class PropertiesConsumer {
 protected:
  /**
   * @brief Reads properties from 0xBx message data.
   * @param s Reference to `MideaData` instance.
   * @return If zero - no more data is required. Else `ID` of next message.
   */
  unsigned m_update(const MideaData &s);

  /**
   * @brief Pure virtual method called on every property while update.
   * @param x Property instance.
   */
  virtual void m_onProperty(const Property &x) = 0;
};

}  // namespace midea
