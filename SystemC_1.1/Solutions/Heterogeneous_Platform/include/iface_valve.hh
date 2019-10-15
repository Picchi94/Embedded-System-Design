#ifndef IFACE_VALVE_HH
#define IFACE_VALVE_HH

#include <systemc-ams.h>
#include "command.hh"

SCA_TDF_MODULE(iface_valve) {
public:
  sca_tdf::sca_out<double> ams_aperture_tr;
  sca_tdf::sca_out<STATE> ams_command;

  //sca_tdf::sca_de::sca_in<STATE> command;
  sca_tdf::sca_de::sca_in<sc_uint<32> > command;

  sca_tdf::sca_de::sca_in<double> aperture_tr;

  explicit iface_valve(sc_core::sc_module_name);

  void processing();
};

#endif
