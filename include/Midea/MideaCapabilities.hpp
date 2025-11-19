#pragma once

#include "Midea/Message/PropertiesConsumer.hpp"

namespace midea {

/**
 * @brief Capabilities base class.
 *
 */
class MideaCapabilities : public PropertiesConsumer {
 public:
  /**
   * @brief Dump capabilities report to logs.
   *
   */
  virtual void dump() const = 0;

  /**
   * @brief Checks if any of 0xB1 capabilities is supported.
   *
   * @return `true` if any of 0xB1 capabilities is supported.
   */
  virtual bool isQueryNeeded() const = 0;
};

}  // namespace midea
