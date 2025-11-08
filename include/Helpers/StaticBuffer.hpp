#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <utility>
#include <string>
#include <cstring>

namespace dongle {

enum MideaType : uint8_t {
  SET_STATUS = 0x40,
  GET_STATUS = 0x41,
  SET_PROPERTIES = 0xB0,
  GET_PROPERTIES = 0xB1,
  GET_CAPABILITIES = 0xB5,
};

class FrameHeader;

class ITransparentDataHandler {
 public:
  /**
   * @brief Frame handler.
   * @param message Reference to the transparent message.
   */
  virtual void frameHandler(const FrameHeader &message) = 0;
  virtual void finalize() = 0;
};

template<size_t N> using array_t = std::array<uint8_t, N>;

class FrameHeader {
 public:
  /**
   * @brief Returns pointer to start of message.
   * @return Pointer to start of message.
   */
  uint8_t *begin() { return &m_start; }

  /**
   * @brief Returns pointer to end of message.
   * @return Pointer to end of message.
   */
  uint8_t *end() { return &m_length + m_length; }

  /**
   * @brief Returns constant pointer to start of message.
   * @return Constant pointer to start of message.
   */
  const uint8_t *begin() const { return &m_start; }

  /**
   * @brief Returns constant pointer to end of message.
   * @return Constant pointer to end of message.
   */
  const uint8_t *end() const { return &m_length + m_length; }

  /**
   * @brief Returns size of message.
   * @return Size of message.
   */
  size_t size() const { return std::distance(begin(), end()); }

  /**
   * @brief Returns pointer to start of message data.
   * @return Pointer to start of message data.
   */
  uint8_t *data_begin() { return &m_start + sizeof(*this); }

  /**
   * @brief Returns pointer to end of message data.
   * @return Pointer to end of message data.
   */
  uint8_t *data_end() { return &m_start + m_length; }

  /**
   * @brief Returns constant pointer to start of message data.
   * @return Constant pointer to start of message data.
   */
  const uint8_t *data_begin() const { return &m_start + sizeof(*this); }

  /**
   * @brief Returns constant pointer to end of message data.
   * @return Constant pointer to end of message data.
   */
  const uint8_t *data_end() const { return &m_start + m_length; }

  /**
   * @brief Returns size of message data.
   * @return Size of message data.
   */
  size_t data_size() const { return std::distance(data_begin(), data_end()); }

  /**
   * @brief Prints message bytes as hex `std::string`.
   * @param separator Bytes separator.
   * @return Frame bytes as hex string.
   */
  std::string hex(char separator = ' ') const;

  /**
   * @brief Access for element by index.
   *
   * @param index index of element.
   * @return uint8_t& element.
   */
  uint8_t &operator[](size_t index) { return m_at(index); }

  /**
   * @brief Access for const element by index.
   *
   * @param index index of element.
   * @return const uint8_t& element.
   */
  const uint8_t &operator[](size_t index) const { return m_at(index); }

  /**
   * @brief Get value from data body.
   *
   * @param index byte index.
   * @param rshift number of bits right shift (default: 0).
   * @param mask value mask (default: 0xFF).
   * @return Value.
   */
  uint8_t getValue(size_t index, uint8_t rshift = 0, uint8_t mask = 255) const {
    return (m_at(index) >> rshift) & mask;
  }

  /**
   * @brief Get bit value from data body.
   *
   * @param index byte index.
   * @param nbit bit number.
   * @return Bit state.
   */
  bool getBit(size_t index, uint8_t nbit) const { return m_at(index) & (1 << nbit); }

  /**
   * @brief Set value to data body.
   *
   * @param index byte index.
   * @param value value without shift.
   * @param mask value mask without bits shift (default: 0xFF). Used to clear destination bits.
   * @param lshift number of bits to left shift value and mask.
   */
  void setValue(size_t index, uint8_t value, uint8_t mask = 255, uint8_t lshift = 0) {
    uint8_t &x = m_at(index);
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

  template<typename T> T &ref(size_t index = 0) { return reinterpret_cast<T &>(m_at(index)); }
  template<typename T> const T &ref(size_t index = 0) const { return reinterpret_cast<const T &>(m_at(index)); }

 protected:
  FrameHeader() = default;
  FrameHeader(uint8_t length, uint8_t appID, uint8_t frameID, uint8_t protoID, uint8_t typeID);

  /**
   * @brief Access for element by index.
   *
   * @param index index of element.
   * @return uint8_t& element.
   */
  uint8_t &m_at(size_t index) { return data_begin()[index]; }

  /**
   * @brief Access for const element by index.
   *
   * @param index index of element.
   * @return const uint8_t& element.
   */
  const uint8_t &m_at(size_t index) const { return data_begin()[index]; }

  /**
   * @brief Returns reference to checksum byte.
   * @return Reference to checksum byte.
   */
  uint8_t &m_checksum() { return *data_end(); }

  /**
   * @brief Returns constant reference to checksum byte.
   * @return Constant reference to checksum byte.
   */
  const uint8_t &m_checksum() const { return *data_end(); }

  /**
   * @brief Calculates and returns checksum.
   * @return Checksum.
   */
  uint8_t m_calcChecksum() const;

  /**
   * @brief Is message transparent.
   * @return Frame transparency.
   */
  bool m_transparent() const;

 protected:
  uint8_t m_start;      // Synchronization header. Always `0xAA`.
  uint8_t m_length;     // Frame length without synchronization header.
  uint8_t m_appliance;  // Home appliance type.
  uint8_t m_sync;       // Frame synchronization check.

 private:
  uint8_t m_reserved1;  // Reserved. Always `0x00`.
  uint8_t m_reserved2;  // Reserved. Always `0x00`.

 protected:
  uint8_t m_id;  // Frame identifier.

 private:
  uint8_t m_reserved3;  // Reserved. Always `0x00`.

 protected:
  uint8_t m_protocol;  // Appliance protocol version.
  uint8_t m_type;      // Frame type identifier.
  // MANDATORY FRAME DATA :
  // 1. APPLIANCE PROTOCOL DATA (0..245 bytes)
  // 2. CHECKSUM BYTE
};

template<typename T> class StaticFrame : public FrameHeader {
  static constexpr size_t LEN = sizeof(FrameHeader) + sizeof(T);

 public:
  /**
   * @brief Constructs message with template data from bytes initializer list.
   * @param appID appliance type identifier.
   * @param frameID message identifier.
   * @param protoID appliance protocol version identifier.
   * @param typeID message type identifier.
   */
  StaticFrame(uint8_t appID, uint8_t frameID, uint8_t protoID, uint8_t typeID)
      : FrameHeader(LEN, appID, frameID, protoID, typeID) {}

  /**
   * @brief Constructs message with template data from bytes initializer list.
   * @param appID appliance type identifier.
   * @param frameID message identifier.
   * @param protoID appliance protocol version identifier.
   * @param typeID message type identifier.
   * @param args message data initializer list.
   */
  StaticFrame(uint8_t appID, uint8_t frameID, uint8_t protoID, uint8_t typeID, std::initializer_list<uint8_t> args)
      : FrameHeader(LEN, appID, frameID, protoID, typeID), m_data(args) {}

  /**
   * @brief Constructs message with template data.
   * @tparam ...Args types of message data initialization arguments.
   * @param appID appliance type identifier.
   * @param frameID message identifier.
   * @param protoID appliance protocol version identifier.
   * @param typeID message type identifier.
   * @param ...args message data initialization arguments.
   */
  template<typename... Args>
  StaticFrame(uint8_t appID, uint8_t frameID, uint8_t protoID, uint8_t typeID, Args... args)
      : FrameHeader(LEN, appID, frameID, protoID, typeID), m_data(args...) {}

  /**
   * @brief Returns reference to message data.
   * @return Reference to message data.
   */
  T &data() { return m_data; }

  /**
   * @brief Returns constant reference to message data.
   * @return Constant reference to message data.
   */
  const T &data() const { return m_data; }

 protected:
  T m_data;  // Frame data. To avoid alignments, it must consist of byte fields only.

 private:
  uint8_t m_reserved4;  // reserved for message checksum
};

class MideaDataTail {
  uint8_t m_id;
  uint8_t m_checksum;

 protected:
  void m_finalize(const uint8_t *data);
  uint8_t m_calcChecksum(const uint8_t *data) const;
};

template<typename T> class MideaData : public T, private MideaDataTail {
 public:
  bool check() const { return m_calcChecksum(reinterpret_cast<const uint8_t *>(this)) == m_checksum; }
  void finalize() { m_finalize(reinterpret_cast<const uint8_t *>(this)); }
};

}  // namespace dongle
