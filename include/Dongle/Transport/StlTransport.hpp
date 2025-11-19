#pragma once

#include <iostream>

#include "Dongle/Transport/ITransport.hpp"

namespace dongle {

/**
 * @brief STL stream transport implementation.
 */
class StlTransport : public ITransport {
 public:
  /**
   * @brief Constructor.
   * @param io STL stream instance.
   */
  StlTransport(std::iostream &io) : m_io(io) {}

  /**
   * @brief Reads new byte from stream if available.
   * @param data destination byte.
   * @return `true` on success.
   */
  bool read(char &data) final override { return static_cast<bool>(m_io.get(data)); }

  /**
   * @brief Writes data to the stream.
   * @param data pointer to data array.
   * @param data_length length of array.
   * @return `true` on success.
   */
  bool write(const char *data, size_t data_length) final override {
    return static_cast<bool>(m_io.write(data, data_length));
  }

 private:
  /// STL stream instance.
  std::iostream &m_io;
};

}  // namespace dongle
