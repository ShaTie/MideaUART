#pragma once

#include <cstdint>
#include <functional>

#include "Platforms.h"

namespace helpers {

class Timer;
using TimerTick = unsigned long;
using TimerCallback = std::function<void(Timer *)>;

class Timer {
 public:
  void setCallback(TimerCallback callback) { m_callback = std::move(callback ? callback : s_dummyCallback); }
  void call() { m_callback(this); }

#ifdef USE_FREERTOS

  Timer(TimerTick timeout, bool reload, TimerCallback callback)
      : m_handle(xTimerCreateStatic(NULL, pdMS_TO_TICKS(timeout), reload, this, s_osCallback, &m_timer)),
        m_callback(std::move(callback ? callback : s_dummyCallback)) {}

  ~Timer() { xTimerDelete(m_handle, portMAX_DELAY); }

  void start(TimerTick timeout) const { xTimerStart(m_handle, pdMS_TO_TICKS(timeout)); }

  void reset() const { xTimerReset(m_handle, portMAX_DELAY); }

  void stop() const { xTimerStop(m_handle, portMAX_DELAY); }

  void setReload(bool reload) const { vTimerSetReloadMode(m_handle, reload); }

  void run() const {}

 private:
  const TimerHandle_t m_handle;
  TimerCallback m_callback;
  StaticTimer_t m_timer;
  static void s_osCallback(TimerHandle_t x);

#else

  Timer(TimerTick timeout, bool reload, TimerCallback callback)
      : m_timeout(timeout), m_callback(callback ? callback : s_dummyCallback), m_active(false), m_reload(reload) {}

  void start(TimerTick timeout) {
    m_timeout = timeout;
    this->reset();
  }

  void reset() {
    m_last = millis();
    m_active = true;
  }

  void stop() { m_active = false; }

  void setReload(bool reload) { m_reload = reload; }

  void run();

 private:
  TimerTick m_timeout, m_last;
  TimerCallback m_callback;
  bool m_active, m_reload;
#endif

#ifdef USE_NATIVE
  TimerTick millis() { return 0; }
#endif

  static void s_dummyCallback(Timer *);
};

}  // namespace helpers
