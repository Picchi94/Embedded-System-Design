#include "controller.hh"

controller::controller(sc_core::sc_module_name) :
  command("command"), aperture_tr("aperture_tr"), input_water("input_water") {
    i = 0;
    t = 0.7;
  }

void controller::set_attributes() {
  this -> set_timestep(4, SC_MS);
  input_water.set_delay(1);
  s = 1100;
}

void controller::processing() {

  double wl = input_water.read();

  if (wl >= 5 && wl <= 8.8) {
    command = IDLE;
    i = 0;
  } else if (wl > 8.8 && i == s) {
    command = CLOSE;
    t *= 0.7;
    i = 0;
  } else if (wl < 5 && i == s) {
    command = OPEN;
    t *= 1.1;
    i = 0;
  }
  aperture_tr = t;
  i++;
}
