#include "w_system.hh"

void w_system::clock() {
  while(true) {
    clk.write(true);
    wait(100, SC_US);
    clk.write(false);
    wait(100, SC_US);
  }
}

w_system::w_system(sc_core::sc_module_name name) :
  sc_module(name),
  watertank_module("watertank_module"),
  valve_module("valve_module"),
  controller_module("controller_module"),
  xtea_module("xtea_module"),
  watertank_transactor_module("watertank_transactor_module"),
  xtea_transactor_module("xtea_transactor_module"),
  iface_valve_module("iface_valve_module"),
  iface_watertank_module("iface_watertank_module") {

    valve_module.ams_command(ams_command);
    valve_module.ams_aperture_tr(ams_aperture_tr);
    valve_module.aperture_ams(ams_valve_aperture);

    watertank_module.aperture_ams(ams_valve_aperture);
    watertank_module.input_water_ams(input_water_ams);

    iface_watertank_module.input_water_ams(input_water_ams);
    iface_watertank_module.input_water(input_water);

    watertank_transactor_module.input_water(input_water);

    controller_module.wtank_socket(watertank_transactor_module.target_socket);
    controller_module.xtea_socket(xtea_transactor_module.target_socket);
    controller_module.aperture_tr(aperture_tr);

    xtea_transactor_module.clk(clk);
    xtea_transactor_module.rst(rst);
    xtea_transactor_module.input_rdy(input_rdy);
    xtea_transactor_module.output_rdy(output_rdy);
    xtea_transactor_module.word1(word1);
    xtea_transactor_module.word2(word2);
    xtea_transactor_module.key0(key0);
    xtea_transactor_module.key1(key1);
    xtea_transactor_module.key2(key2);
    xtea_transactor_module.key3(key3);
    xtea_transactor_module.mode(mode);

    xtea_module.clk(clk);
    xtea_module.rst(rst);
    xtea_module.input_rdy(input_rdy);
    xtea_module.output_rdy(output_rdy);
    xtea_module.word1(word1);
    xtea_module.word2(word2);
    xtea_module.mode(mode);
    xtea_module.key0(key0);
    xtea_module.key1(key1);
    xtea_module.key2(key2);
    xtea_module.key3(key3);
    xtea_module.result0(command);
    xtea_module.result1(result1);

    iface_valve_module.command(command);
    iface_valve_module.ams_command(ams_command);
    iface_valve_module.ams_aperture_tr(ams_aperture_tr);
    iface_valve_module.aperture_tr(aperture_tr);

    SC_THREAD(clock);

  }
