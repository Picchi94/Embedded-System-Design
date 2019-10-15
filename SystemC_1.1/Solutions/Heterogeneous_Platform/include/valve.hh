#ifndef VALVE_HH
#define VALVE_HH

#include <systemc-ams.h>
#include "command.hh"

SCA_TDF_MODULE(valve) {
public:
  sca_tdf::sca_in<double> ams_aperture_tr;
  sca_tdf::sca_in<STATE> ams_command;

  sca_tdf::sca_out<double> aperture_ams;

  explicit valve(sc_core::sc_module_name);

  void processing() override;

  void set_attributes() override;
private:
  double valve_aperture; //current aperture
  double threshold_aperture; //current threshold
};

#endif
