#include "w_system.hh"

int sc_main(int argc, char *argv[]) {
  w_system s("w_system");

  sca_trace_file *file = sca_create_vcd_trace_file("waterwave");

  sca_trace(file, s.input_water, "input_water");
  sca_trace(file, s.ams_aperture_tr, "aperture_threshold");
  sca_trace(file, s.ams_valve_aperture, "ams_valve_aperture");

  sc_start(600, SC_SEC);
  sca_close_vcd_trace_file(file);
}
