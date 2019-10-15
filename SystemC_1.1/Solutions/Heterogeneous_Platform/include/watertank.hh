#ifndef WATERTANK_HH
#define WATERTANK_HH

#include <systemc-ams.h>
//#include "valve.hh"

class watertank : public sc_core::sc_module {

public:

  sca_tdf::sca_out<double> input_water_ams;

  sca_tdf::sca_in<double> aperture_ams;

  explicit watertank(sc_module_name);

private:

  sca_lsf::sca_tdf::sca_source converter_in;
  sca_lsf::sca_tdf::sca_sink converter_out;

  sca_lsf::sca_gain gain1, gain2;
  sca_lsf::sca_sub sub;
  sca_lsf::sca_integ integ;

  sca_lsf::sca_signal sig1, sig2, sig3, sig4, sig5;
};

#endif
