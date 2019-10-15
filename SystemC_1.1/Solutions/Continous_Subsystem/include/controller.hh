#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include "command.hh"
#include "watertank.hh"

SCA_TDF_MODULE(controller) {
public:
  sca_tdf::sca_in<double> input_water;
  sca_tdf::sca_out<double> aperture_tr;
  sca_tdf::sca_out<STATE> command;

  explicit SCA_CTOR(controller);

  void set_attributes();
  void processing();
private:
  double t;
  int s, i;
};

#endif
