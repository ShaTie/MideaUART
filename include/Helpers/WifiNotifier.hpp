#pragma once

#include <cstdint>
#include <functional>

#include "Helpers/Timer.hpp"

#if defined(IDF_VER)
#include "esp_netif.h"
#include "esp_wifi.h"
#elif defined(USE_ARDUINO)
#include "ESP8266WiFi.h"
#elif not defined(USE_NATIVE)
#error "Current platform not supported"
#endif

namespace helpers {

class WifiNotifierBase;
using WifiNotifyCallback = std::function<void(const WifiNotifierBase &)>;

class WifiNotifierBase {
 public:
  /**
   * @brief Base WiFi notifier.
   * @param period Automatic update period in milliseconds.
   */
  WifiNotifierBase(TimerTick period);
  /**
   * @brief Is connected to AP.
   * @return Connection state.
   */
  bool connected() const { return m_ip; }
  /**
   * @brief IPv4 address.
   * @return IPv4 address.
   */
  uint32_t ip() const { return m_ip; }
  /**
   * @brief Current WiFi RSSI.
   * @return WiFi signal strength.
   */
  int level() const;
  /**
   * @brief Run internal update timer.
   */
  void run() { m_timer.run(); }

 private:
  /// IPv4 address
  uint32_t m_ip;
  /// Update timer
  Timer m_timer;
  /// Update method
  void m_update(uint32_t ip);
  /// Virtual update method
  virtual void m_update() = 0;
#ifdef IDF_VER
  /// IDF network event handler
  static void s_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
#endif
};

class WifiNotifier : public WifiNotifierBase {
 public:
  /**
   * @brief Base WiFi notifier.
   * @param period Automatic update period in milliseconds.
   * @param callback Update callback.
   */
  WifiNotifier(TimerTick period, WifiNotifyCallback callback) : WifiNotifierBase(period), m_callback(callback) {}
  /**
   * @brief Set update callback function.
   * @param callback Update callback.
   */
  void setCallback(WifiNotifyCallback callback) { m_callback = callback; }

 private:
  /// Update callback
  WifiNotifyCallback m_callback;
  /// Update method
  void m_update() override { m_callback(*this); }
};

}  // namespace helpers
