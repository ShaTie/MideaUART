#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <initializer_list>

namespace dongle {

class DataManager {
 public:
  DataManager() : m_data(0) {}
  /**
   * @brief Access for const element by index.
   *
   * @param index index of element.
   * @return const uint8_t& element.
   */
  const uint8_t &operator[](size_t index) const { return m_data[index]; }

  /**
   * @brief Access for element by index.
   *
   * @param index index of element.
   * @return uint8_t& element.
   */
  uint8_t &operator[](size_t index) { return m_data[index]; }

  /**
   * @brief Get value from data body.
   *
   * @param index byte index.
   * @param rshift number of bits right shift (default: 0).
   * @param mask value mask (default: 0xFF).
   * @return Value.
   */
  uint8_t getValue(size_t index, uint8_t rshift = 0, uint8_t mask = 255) const {
    return (m_data[index] >> rshift) & mask;
  }

  /**
   * @brief Get bit value from data body.
   *
   * @param index byte index.
   * @param nbit bit number.
   * @return Bit state.
   */
  bool getBit(size_t index, uint8_t nbit) const { return m_data[index] & (1 << nbit); }

  /**
   * @brief Set value to data body.
   *
   * @param index byte index.
   * @param value value without shift.
   * @param mask value mask without bits shift (default: 0xFF). Used to clear destination bits.
   * @param lshift number of bits to left shift value and mask.
   */
  void setValue(size_t index, uint8_t value, uint8_t mask = 255, uint8_t lshift = 0) {
    auto &x(m_data[index]);
    x &= ~(mask << lshift);
    x |= (value & mask) << lshift;
  }

  /**
   * @brief Set or clear bitmask.
   *
   * @param index byte index.
   * @param state if `true` bitmask is set, else clear.
   * @param mask bitmask.
   */
  void setMask(size_t index, uint8_t mask, bool state) { setValue(index, state ? mask : 0, mask); }

  /**
   * @brief Set bit state.
   *
   *
   * @param index byte index.
   * @param nbit bit number.
   * @param state bit state.
   */
  void setBit(size_t index, uint8_t nbit, bool state) { setMask(index, 1 << nbit, state); }

  template<typename T> T &ref(size_t index = 0) { return reinterpret_cast<T &>(m_data[index]); }
  template<typename T> const T &ref(size_t index = 0) const { return reinterpret_cast<const T &>(m_data[index]); }
  template<typename T> T *ptr(size_t index = 0) { return reinterpret_cast<T *>(&m_data[index]); }
  template<typename T> const T *ptr(size_t index = 0) const { return reinterpret_cast<const T *>(&m_data[index]); }

 private:
  uint8_t *const m_data;
};

/**
 * @brief Message body.
 *
 */
class Buffer {
 public:
  using container_t = std::vector<uint8_t>;

  /**
   * @brief Default constructor. Data vector is empty.
   */
  Buffer() = default;

  /**
   * @brief Constructor from r-value vector.
   * @param x R-value vector.
   */
  explicit Buffer(container_t &&x) : m_data(std::move(x)) {}

  /**
   * @brief Constructor from initializer list. Used for brace initialization.
   * @param x initializer list.
   */
  Buffer(std::initializer_list<uint8_t> x) : m_data(std::move(x)) {}

  /**
   * @brief Default constructor. Data vector is empty.
   */
  explicit Buffer(size_t initial_size) : m_data(initial_size) {}

  /**
   * @brief Constructs data body from begin and end data pointers.
   *
   * @param begin begin iterator.
   * @param end end iterator.
   */
  explicit Buffer(container_t::const_iterator begin, container_t::const_iterator end) : m_data(begin, end) {}

  /**
   * @brief Move this object to derivied message data object.
   *
   * @tparam T message data class type.
   * @return New data message object.
   */
  template<typename T> T to() { return T(std::move(m_data)); }

  /**
   * @brief Access for const element by index.
   *
   * @param index index of element.
   * @return const uint8_t& element.
   */
  const uint8_t &operator[](size_t index) const { return m_data[index]; }

  /**
   * @brief Access for element by index.
   *
   * @param index index of element.
   * @return uint8_t& element.
   */
  uint8_t &operator[](size_t index) { return m_data[index]; }

  /**
   * @brief Returns buffer data vector.
   * @return buffer data vector.
   */
  const container_t &buffer() const { return m_data; }

  /**
   * @brief Appends data (little-endian byte order).
   *
   * @tparam T integer data type.
   * @param value value to append.
   */
  template<typename T> void append(const T &value) {
    for (auto n = 0; n != sizeof(T); ++n)
      m_data.push_back(value >> (n * 8));
  }

  /**
   * @brief Appends variadic number of integer arguments.
   *
   * @param value first argument.
   * @param ...args other arguments.
   */
  template<typename T, typename... Args> void append(const T &value, Args... args) {
    this->append(value);
    this->append(args...);
  }

  /**
   * @brief Get value from data body.
   *
   * @param index byte index.
   * @param rshift number of bits right shift (default: 0).
   * @param mask value mask (default: 0xFF).
   * @return Value.
   */
  uint8_t getValue(size_t index, uint8_t rshift = 0, uint8_t mask = 255) const {
    return (m_data[index] >> rshift) & mask;
  }

  /**
   * @brief Get bit value from data body.
   *
   * @param index byte index.
   * @param nbit bit number.
   * @return Bit state.
   */
  bool getBit(size_t index, uint8_t nbit) const { return m_data[index] & (1 << nbit); }

  /**
   * @brief Set value to data body.
   *
   * @param index byte index.
   * @param value value without shift.
   * @param mask value mask without bits shift (default: 0xFF). Used to clear destination bits.
   * @param lshift number of bits to left shift value and mask.
   */
  void setValue(size_t index, uint8_t value, uint8_t mask = 255, uint8_t lshift = 0) {
    uint8_t &x = m_data[index];
    x &= ~(mask << lshift);
    x |= (value & mask) << lshift;
  }

  /**
   * @brief Set or clear bitmask.
   *
   * @param index byte index.
   * @param state if `true` bitmask is set, else clear.
   * @param mask bitmask.
   */
  void setMask(size_t index, uint8_t mask, bool state) { setValue(index, state ? mask : 0, mask); }

  /**
   * @brief Set bit state.
   *
   *
   * @param index byte index.
   * @param nbit bit number.
   * @param state bit state.
   */
  void setBit(size_t index, uint8_t nbit, bool state) { setMask(index, 1 << nbit, state); }

  template<typename T> T &ref(size_t index = 0) { return reinterpret_cast<T &>(m_data[index]); }
  template<typename T> const T &ref(size_t index = 0) const { return reinterpret_cast<const T &>(m_data[index]); }

 protected:
  /**
   * @brief Data vector.
   *
   */
  container_t m_data;
};

}  // namespace dongle
