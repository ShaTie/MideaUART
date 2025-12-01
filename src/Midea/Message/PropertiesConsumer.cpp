#include "Midea/Message/PropertiesConsumer.hpp"

namespace midea {

class PropertiesReader : public Property {
 public:
  PropertiesReader(const MideaData &x) : Property(x), m_end(&x.buffer().back()) {}
  PropertiesReader(const PropertiesReader &) = delete;

  unsigned nextID() const { return (m_header != m_end) ? m_header[0] : 0; }

  bool valid() const { return std::distance(m_data, m_end) >= size(); }

  void advance() {
    auto len(std::distance(m_header, m_data));
    m_header = m_data + size();
    m_data = m_header + len;
  }

 private:
  const uint8_t *const m_end;
};

unsigned PropertiesConsumer::m_update(const MideaData &x) {
  PropertiesReader reader(x);

  for (; reader.valid(); reader.advance())
    m_onProperty(reader);

  return reader.nextID();
}

}  // namespace midea
