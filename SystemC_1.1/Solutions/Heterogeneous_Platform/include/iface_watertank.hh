#ifndef IFACE_WATERTANK_HH
#define IFACE_WATERTANK_HH

#include <systemc-ams.h>
#include "command.hh"
#include "watertank.hh"

SCA_TDF_MODULE(iface_watertank){
public:
  sca_tdf::sca_in<double> input_water_ams;

  sca_tdf::sca_de::sca_out<double> input_water;

  explicit iface_watertank(sc_core::sc_module_name);

  void processing();

};

#endif
