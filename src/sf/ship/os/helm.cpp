#include "helm.hpp"

using namespace shipos;

Helm::Helm(Ship* ship)
    : Program(ship, "Helm"), autopilot(false), ptr_engine(0) {}

Helm::Helm(Ship* ship, WindowAlignment alignment_x, WindowAlignment alignment_y,
           double size_x, double size_y)
    : Program(ship, "Helm", alignment_x, alignment_y, size_x, size_y),
      autopilot(false),
      engpwr(1),
      ptr_engine(nullptr) {
  this->window->setTitle(Lang::get("program_helm"));
}

void Helm::render(ConsoleKey key) {
  // find modules
  if (this->ptr_engine == nullptr)
    this->ptr_engine = (module::Engine*)this->ship->getFirstModule("Engine");

  werase(this->win);
  this->getWindowSize();

  // disable keyboard control when docked
  if (this->ship->isDocked()) key = ConsoleKey::NONE;

  // get rotation from ship
  this->rot = this->ship->getRot();

  // parse keys
  if ((char)key == 'a' || key == ConsoleKey::ARROW_LEFT) {
    this->rot -= 0.05;
    this->autopilot = false;
  }
  if ((char)key == 'd' || key == ConsoleKey::ARROW_RIGHT) {
    this->rot += 0.05;
    this->autopilot = false;
  }
  if ((char)key == 'w' || key == ConsoleKey::ARROW_UP) {
    // fire engines
    this->setThrust(Vec2::fromAngle(this->rot, this->engpwr));
    this->autopilot = false;
  }
  if ((char)key == 'y') {
    this->autopilot = true;
  }
  if ((char)key == 'q' && this->engpwr >= 0.1) {
    this->engpwr -= 0.1;
    this->autopilot = false;
  }
  if ((char)key == 'e' && this->engpwr <= 0.9) {
    this->engpwr += 0.1;
    this->autopilot = false;
  }

  // correct rot variable
  if (this->rot < 0) {
    this->rot += 2 * 3.1416;
  }
  if (this->rot > 2 * 3.1416) {
    this->rot -= 2 * 3.1416;
  }

  // autopilot
  if (this->autopilot) {
    // get vel vector
    Vec2 ivel = this->ship->getVel().inverse();
    // calc vel angle
    double ang = ivel.angle();

    // get angular vector
    double avec = ang - this->rot;
    if (avec > 0) {
      avec = fmin(avec, 0.05);
    } else {
      avec = fmax(avec, -0.05);
    }

    this->rot += avec;

    mvwprintw(this->win, 2, 1, "%.0f", ang / 3.1416 * 180.0);

    // thrust
    if (fabs(avec) < 0.001) {
      double velabs = ivel.magnitude() * 1000.0 * 5;
      velabs = fmin(velabs, 1.0);

      // turn ap off when stopped
      if (velabs <= 0.01) {
        this->autopilot = false;
      } else {
        this->setThrust(Vec2::fromAngle(this->rot, velabs));
        this->engpwr = floor(velabs * 10) / 10.0;
      }
    }
  }

  // print angle
  mvwprintw(this->win, 1, 1, "%.0f", this->rot / 3.1416 * 180.0);

  // draw center
  int cx = round(this->wwidth / 2.0f) - 1;
  int cy = round(this->wheight / 2.0f) - 2;
  mvwprintw(this->win, cy, cx, "O");
  double r = fmin((this->wwidth - 4) / 2.0, this->wheight - 6) / 2.0;

  // draw circle
  int steps = round(r * 20);
  for (int i = 0; i < steps; i++) {
    int px = round(sin((double)i + 3.1416 * 2 / (double)steps) * r * 2 + cx);
    int py = round(-cos((double)i + 3.1416 * 2 / (double)steps) * r + cy);
    mvwprintw(this->win, py, px, "X");
  }
  // draw line
  int stepsl = round(r);
  for (int i = 0; i < stepsl; i++) {
    int px =
        round(sin(this->rot) * r * 2.0 * ((double)i / (double)stepsl) + cx);
    int py = round(-cos(this->rot) * r * ((double)i / (double)stepsl) + cy);
    mvwprintw(this->win, py, px, "o");
  }

  // draw eng power
  if (this->ptr_engine != 0) {
    if (this->ptr_engine->isOnline()) {
      std::string pwr = Lang::get("program_helm_eng") + ": [";
      int nsymb = this->wwidth - 3 - pwr.length();
      int nsymb_h = round(nsymb * this->engpwr);
      for (int i = 0; i < nsymb_h; i++) {
        pwr += "#";
      }
      for (int i = 0; i < nsymb - nsymb_h; i++) {
        pwr += "_";
      }
      pwr += "]";
      mvwprintw(this->win, this->wheight - 2, 1, pwr.c_str());
      wattron(this->win, A_BOLD);
      mvwprintw(this->win, this->wheight - 2, cx + 1, "%.0f%%", engpwr * 100.0);
      wattroff(this->win, A_BOLD);
    } else {
      // print error if engine is offline
      this->showError(this->wheight - 4, Lang::get("program_helm_engoffline"));
    }
  } else {
    // print error if engine does not exist
    this->showError(this->wheight - 4, Lang::get("program_helm_noengine"));
  }

  // draw autopilot active
  if (this->autopilot) {
    this->showCentered(this->wheight - 3, Lang::get("program_helm_autopilot"),
                       A_BOLD);
  }

  // apply rotation
  this->ship->setRot(this->rot);

  // render window
  Program::render(key);
}

void Helm::setThrust(const Vec2& thr) {
  if (this->ptr_engine == 0) return;
  this->ptr_engine->setThrust(thr);
}
