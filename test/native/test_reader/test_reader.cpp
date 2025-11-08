#include "unity.h"
#include <fstream>

#include "Dongle/Message/MessageReaderWriter.hpp"
#include "Dongle/Transport/StlTransport.hpp"

using namespace dongle;

void setUp() {}
void tearDown() {}

void test_default_constructor() {}

static Message msg_from_file(const char *path) {
  while (true) {
    std::fstream file(path, std::ios::in | std::ios::binary);

    if (!file)
      break;

    StlTransport transport(file);
    MessageReaderWriter reader(&transport);

    if (!reader.read())
      break;

    return reader;
  }

  TEST_FAIL_MESSAGE("Failed to read message from file.");
}

void test_constructor() {
  auto msg(msg_from_file("D:\\Develop\\MideaUART\\messages\\msg-0xC0-1761040882.bin"));
  std::ofstream txt("D:\\Develop\\MideaUART\\messages\\msg-0xC0-1761040882.txt");
  txt << msg.hex();
}

int runUnityTests() {
  UNITY_BEGIN();
  RUN_TEST(test_default_constructor);
  RUN_TEST(test_constructor);
  return UNITY_END();
}

int main() { return runUnityTests(); }
