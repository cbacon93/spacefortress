#include "generator.hpp"

using namespace module;

Generator::Generator(const std::string& name, double hull, double power_output)
    : Module(name, "Generator", hull),
      max_power_output(power_output),
      last_power_usage(0),
      last_power_generation(0) {}

void Generator::simulate(double delta_time, Ship* ship) {
  // calc last power usage
  this->last_power_usage = this->last_power_generation - this->energy_available;

  this->energy_available = 0;
  if (!this->online) {
    return;
  }

  // try to add max power
  this->energy_available = max_power_output * delta_time;
  this->last_power_generation = this->energy_available;
}
