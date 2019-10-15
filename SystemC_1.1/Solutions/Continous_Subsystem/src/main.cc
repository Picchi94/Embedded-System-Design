#include "valve.hh"
#include "watertank.hh"
#include "controller.hh"

int sc_main(int argc, char *argv[]) {
  sca_tdf::sca_signal<double> input_water;
  sca_tdf::sca_signal<double> aperture_tr;
  sca_tdf::sca_signal<double> valve_aperture;

  sca_tdf::sca_signal<STATE> command;

  valve valve("valve");
  watertank watertank("input_water");
  controller controller("controller");

  valve.command(command);
  valve.aperture_tr(aperture_tr);
  valve.aperture(valve_aperture);

  watertank.aperture(valve_aperture);
  watertank.input_water(input_water);

  controller.input_water(input_water);
  controller.command(command);
  controller.aperture_tr(aperture_tr);

  sca_trace_file *file = sca_create_vcd_trace_file("waterwave");
  sca_trace(file, command, "command");
  sca_trace(file, aperture_tr, "aperture_tr");
  sca_trace(file, input_water, "input_water");
  sca_trace(file, valve_aperture, "valve_aperture");

  sc_start(600, SC_SEC);
  sca_close_vcd_trace_file(file);
}
