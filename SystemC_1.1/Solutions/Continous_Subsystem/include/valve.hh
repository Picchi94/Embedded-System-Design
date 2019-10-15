#ifndef VALVE_HH
#define VALVE_HH

#include <systemc-ams.h>
#include "command.hh"

SCA_TDF_MODULE(valve) {
public:
  sca_tdf::sca_in<double> aperture_tr;
  sca_tdf::sca_in<STATE> command;

  sca_tdf::sca_out<double> aperture;

  explicit valve(sc_core::sc_module_name);

  void processing();

private:
  double valve_aperture;
};

#endif
