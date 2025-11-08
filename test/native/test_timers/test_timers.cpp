#include "unity.h"
#include "Midea/AirConditioner/Status/DeviceTimers.hpp"

using dongle::midea::ac::DeviceTimers;

void setUp() {}
void tearDown() {}

void test_default_constructor() {
  DeviceTimers timers;
  TEST_ASSERT_EQUAL(0, timers.getTimeOn());
  TEST_ASSERT_EQUAL(0, timers.getTimeOff());
}

void test_constructor() {
  DeviceTimers timers(15, 59);
  TEST_ASSERT_EQUAL(15, timers.getTimeOn());
  TEST_ASSERT_EQUAL(59, timers.getTimeOff());
}

int runUnityTests() {
  UNITY_BEGIN();
  RUN_TEST(test_default_constructor);
  RUN_TEST(test_constructor);
  return UNITY_END();
}

int main() { return runUnityTests(); }
