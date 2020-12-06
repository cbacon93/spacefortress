#include <iostream>

#include "../../src/tools/log.hpp"
#include "../../src/tools/server.hpp"

int main(void) {
  Log log(LogLevel::ALL);
  Server s("127.0.0.1", 8123);
  s.start();

  std::cin.get();

  return 0;
}