#pragma once

#include <cstddef>
#include <cstdint>

namespace dongle {

/**
 * @brief ITransport interface.
 */
class ITransport {
 public:
  /**
   * @brief Reads new byte from stream if available.
   * @param data destination byte.
   * @return `true` on success.
   */
  virtual bool read(char &data) = 0;

  /**
   * @brief Writes data to the stream.
   * @param data pointer to data array.
   * @param data_length length of array.
   * @return `true` on success.
   */
  virtual bool write(const char *data, size_t data_length) = 0;
};

}  // namespace dongle
