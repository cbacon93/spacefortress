#include "statusmonitor.hpp"

shipos::StatusMonitor::StatusMonitor(WINDOW* win, Ship* ship)
    : Program(win, ship) {}

void shipos::StatusMonitor::render(ConsoleKey key) {
  mvwprintw(this->win, 1, 1, "%s:", this->ship->getName().c_str());

  double hull_ratio = this->ship->getHull() / this->ship->getMaxHull();
  if (hull_ratio < 0.1) {
    wattron(this->win, A_BLINK);
  }
  if (hull_ratio < 0.3) {
    wattron(this->win, A_BOLD);
  }
  mvwprintw(this->win, 2, 1, "H: %.1f/%.1f", this->ship->getHull(),
            this->ship->getMaxHull());
  if (hull_ratio < 0.1) {
    wattroff(this->win, A_BLINK);
  }
  if (hull_ratio < 0.3) {
    wattroff(this->win, A_BOLD);
  }

  double shield_ratio = this->ship->getShield() / this->ship->getMaxShield();
  if (shield_ratio < 0.1) {
    wattron(this->win, A_BLINK);
  }
  if (shield_ratio < 0.3) {
    wattron(this->win, A_BOLD);
  }
  mvwprintw(this->win, 3, 1, "S: %.1f/%.1f", this->ship->getShield(),
            this->ship->getMaxShield());
  if (shield_ratio < 0.1) {
    wattroff(this->win, A_BLINK);
  }
  if (shield_ratio < 0.3) {
    wattroff(this->win, A_BOLD);
  }

  double energy_ratio =
      this->ship->getEnergyLevel() / this->ship->getEnergyTotalCapacity();
  if (energy_ratio < 0.1) {
    wattron(this->win, A_BLINK);
  }
  if (energy_ratio < 0.3) {
    wattron(this->win, A_BOLD);
  }
  mvwprintw(this->win, 4, 1, "E: %.1f/%.1f", this->ship->getEnergyLevel(),
            this->ship->getEnergyTotalCapacity());
  if (energy_ratio < 0.1) {
    wattroff(this->win, A_BLINK);
  }
  if (energy_ratio < 0.3) {
    wattroff(this->win, A_BOLD);
  }

  mvwprintw(this->win, 5, 1, "%i Modules installed",
            this->ship->getModuleCount());
}
