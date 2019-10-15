#include "iface_watertank.hh"

iface_watertank::iface_watertank(sc_core::sc_module_name) :
    input_water_ams("input_water_ams"),
    input_water("input_water"){}

void iface_watertank::processing(){
  input_water.write(input_water_ams.read());
}
