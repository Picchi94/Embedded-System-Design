#include "valve.hh"

valve::valve(sc_core::sc_module_name) :
  ams_aperture_tr("ams_aperture_tr"), ams_command("ams_command"), aperture_ams("aperture_ams")
  {}


void valve::processing() {
  threshold_aperture = ams_aperture_tr.read();

  double func = 0.25 * get_timestep().to_seconds();
  switch (ams_command.read()) {
  case STATE::IDLE:
    break;
  case STATE::OPEN:
    if (valve_aperture < threshold_aperture) {
      valve_aperture += func;
  }  else {
      valve_aperture = threshold_aperture;
    }
    break;
  case STATE::CLOSE:
    if (valve_aperture > 0) {
      valve_aperture -= func;
    } else {
      valve_aperture = 0;
    }
    break;
  default:
    break;
  }
  aperture_ams.write(valve_aperture);
}

void valve::set_attributes() {
  this -> set_timestep(1, SC_MS);
}
