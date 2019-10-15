#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <systemc-ams.h>
#include <tlm.h>
#include <tlm_utils/tlm_quantumkeeper.h>
#include "struct.hh"
#include "command.hh"

class controller : public sc_module, public tlm::tlm_bw_transport_if<> {
public:
  SC_HAS_PROCESS(controller);
  explicit controller(sc_module_name name_);

  tlm::tlm_initiator_socket<> wtank_socket;
  tlm::tlm_initiator_socket<> xtea_socket;

  virtual void invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range) override;
  virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t) override;

  sc_core::sc_out<double> aperture_tr;

private:
  wtank_struct wtank_packet;
  xtea_struct xtea_packet;

  sc_time timing_annotation;

  tlm_utils::tlm_quantumkeeper m_qk;

  double t;
  double wl; 

  sc_uint<32> key0 = KEY0;
  sc_uint<32> key1 = KEY1;
  sc_uint<32> key2 = KEY2;
  sc_uint<32> key3 = KEY3;

  void run();
  void encrypt();
};

#endif
