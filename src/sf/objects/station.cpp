#include "station.hpp"

using namespace go;

Station::Station(const std::string& name, double x, double y)
    : GameObject(name, "Station", x, y, true) {
  this->symbol = "I";
  this->is_dockable = true;
}
