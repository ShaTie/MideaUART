#include "Helpers/Timer.hpp"

namespace helpers {

// Dummy function for incorrect using case
void Timer::s_dummyCallback(Timer *) {}

#ifdef USE_FREERTOS
// FreeRTOS callback
void Timer::s_osCallback(TimerHandle_t x) { static_cast<Timer *>(pvTimerGetTimerID(x))->call(); }

#else

void Timer::run() {
  if (!m_active)
    return;

  TimerTick now = millis();

  if ((now - m_last) < m_timeout)
    return;

  m_last = now;
  m_active = m_reload;

  this->call();
}

#endif

}  // namespace dongle
