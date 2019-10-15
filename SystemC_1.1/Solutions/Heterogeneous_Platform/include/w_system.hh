#ifndef W_SYSTEM_HH
#define W_SYSTEM_HH

#include <systemc.h>
#include <systemc-ams.h>
#include <tlm.h>

#include "watertank.hh"
#include "valve.hh"
#include "controller.hh"
#include "xtea_RTL.hh"

#include "watertank_transactor.hh"
#include "xtea_transactor.hh"
#include "iface_valve.hh"
#include "iface_watertank.hh"

class w_system : public sc_module {
public:
  explicit w_system(sc_core::sc_module_name);
  watertank watertank_module;
  valve valve_module;
  controller controller_module;
  xtea_RTL xtea_module;

  watertank_transactor watertank_transactor_module;
  xtea_transactor xtea_transactor_module;
  iface_valve iface_valve_module;
  iface_watertank iface_watertank_module;

  sca_tdf::sca_signal<double> ams_aperture_tr;
  sca_tdf::sca_signal<STATE> ams_command;
  sca_tdf::sca_signal<double> ams_valve_aperture;
  sca_tdf::sca_signal<double> input_water_ams;

  sc_core::sc_signal<double> input_water;
  //sc_core::sc_signal<STATE> command;
  sc_core::sc_signal<sc_uint<32> > command;
  sc_core::sc_signal<double> aperture_tr;

  sc_signal<bool> clk;
  sc_signal<bool> rst;
  sc_signal<sc_uint<1> > input_rdy, output_rdy, mode;
  sc_signal<sc_uint<32> > word1, word2, key0, key1, key2, key3, result0, result1;


  void clock();
  SC_HAS_PROCESS(w_system);

};

#endif
