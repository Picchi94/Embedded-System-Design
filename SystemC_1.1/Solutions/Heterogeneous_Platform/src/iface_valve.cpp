#include "iface_valve.hh"

iface_valve::iface_valve(sc_core::sc_module_name) :
    ams_aperture_tr("ams_aperture_tr"),
    ams_command("ams_command"),
    aperture_tr("aperture_tr"),
    command("command"){}

void iface_valve::processing() {
  ams_aperture_tr.write(aperture_tr.read());
  switch(command.read()) {
    case 1:
          ams_command.write(STATE::IDLE);
    break;
    case 2:
          ams_command.write(STATE::CLOSE);
    break;
    default:
          ams_command.write(STATE::OPEN);
    break;
  }
}
