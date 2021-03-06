#include "star.hpp"

using namespace go;

Star::Star(const std::string &name, double x, double y)
    : GameObject(name, "Star", x, y, true) {
  this->symbol = u8"\u2600";  // 2605 // 26AA
}
