#include "Midea/Message/PropertiesConsumer.hpp"

namespace midea {

class PropertiesReader : public Property {
 public:
  PropertiesReader(const MideaData &x) : Property(x) {}
  PropertiesReader(const PropertiesReader &) = delete;

  unsigned nextID() const { return (m_header != m_end) ? *m_header : 0; }

  void advance() {
    auto len(std::distance(m_header, m_data));
    m_header = m_data + size();
    m_data = m_header + len;
  }
};

unsigned PropertiesConsumer::m_update(const MideaData &x) {
  PropertiesReader reader(x);

  for (; reader.valid(); reader.advance())
    m_onProperty(reader);

  return reader.nextID();
}

}  // namespace midea
