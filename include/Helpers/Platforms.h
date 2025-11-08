#pragma once

#if defined(IDF_VER)
#define USE_FREERTOS
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#elif defined(ARDUINO)
#define USE_ARDUINO
#include "Arduino.h"
#else
#define USE_NATIVE
#endif
