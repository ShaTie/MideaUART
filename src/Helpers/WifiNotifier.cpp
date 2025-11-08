#include "Helpers/WifiNotifier.hpp"

namespace helpers {

static unsigned static_rssi_to_level(int rssi) {
  if (rssi >= -50)
    return 4;

  if (rssi >= -70)
    return 3;

  if (rssi >= -80)
    return 2;

  return 1;
}

static unsigned static_get_level() {
  int rssi;
#if defined(IDF_VER)
  esp_wifi_sta_get_rssi(&rssi);
#elif defined(USE_ARDUINO)
  rssi = WiFi.RSSI();
#endif
  return static_rssi_to_level(rssi);
}

int WifiNotifierBase::level() const { return this->connected() ? static_get_level() : 0; }

WifiNotifierBase::WifiNotifierBase(TimerTick period) : m_ip(0), m_timer(period, true, [this](Timer *) { m_update(); }) {
#if defined(USE_ARDUINO)
  WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP &x) { m_update(x.ip.v4()); });
  WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &) { m_update(0); });
#elif defined(IDF_VER)
  esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &s_handler, this, NULL);
  esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &s_handler, this, NULL);
}

void WifiNotifierBase::s_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
  uint32_t ip = 0;
  if (event_base == IP_EVENT)
    ip = static_cast<ip_event_got_ip_t *>(arg)->ip_info.ip.addr;
  static_cast<WifiNotifierBase *>(event_data)->m_update(ip);
#endif
}

void WifiNotifierBase::m_update(uint32_t ip) {
  if (m_ip == ip)
    return;
  m_ip = ip;
  m_timer.reset();
  m_update();
}

}  // namespace helpers
