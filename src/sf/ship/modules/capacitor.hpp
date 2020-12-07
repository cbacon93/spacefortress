#pragma once

#include <algorithm>

#include "../module.hpp"
#include "../ship.hpp"

namespace module {
class Capacitor : public Module {
 public:
  Capacitor(const std::string &name, double hull, double max_capacity,
            double max_power_input, double max_power_output);
  void simulate(double delta_time, Ship *ship);

  double getMaxCapacity() { return this->max_capacity; }
  double getEnergy(double energy_needed);

 private:
  double max_power_input;
  double max_power_output;
  double max_capacity;

  double power_output_avbl;
};
}  // namespace module