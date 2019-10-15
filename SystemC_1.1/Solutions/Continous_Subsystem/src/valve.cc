#include "valve.hh"

valve::valve(sc_core::sc_module_name) :
  aperture_tr("aperture_tr"), command("command"), aperture("aperture"){}


void valve::processing() {
  double func = 0.25 * get_timestep().to_seconds();
  switch (command.read()) {
  case IDLE:
    break;
  case OPEN:
    if (valve_aperture < aperture_tr.read()) {
      valve_aperture += func;
  }  else {
      valve_aperture = aperture_tr.read();
    }
    break;
  case CLOSE:
    if (valve_aperture > 0) {
      valve_aperture -= func;
    } else {
      valve_aperture = aperture_tr.read();
      aperture.write(0);
    }
    break;
  default:
    break;
  }
  aperture.write(valve_aperture);
}
