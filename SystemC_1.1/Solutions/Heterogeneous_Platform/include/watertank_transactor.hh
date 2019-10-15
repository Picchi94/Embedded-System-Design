#ifndef WATERTANK_TRANSACTOR_HH
#define WATERTANK_TRANSACTOR_HH

#include <systemc-ams.h>
#include <tlm.h>
#include "struct.hh"
#include "iface_watertank.hh"

class watertank_transactor : public sc_module, public virtual tlm::tlm_fw_transport_if<> {
public:

  void b_transport(tlm::tlm_generic_payload &trans, sc_time &t) override;

  bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans,
                          tlm::tlm_dmi &dmi_data) override {return false;};

  unsigned int transport_dbg(tlm::tlm_generic_payload &trans) override{ return 0;};

  void end_of_elaboration() override;

  void transduce();

  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &trans,
                                     tlm::tlm_phase &phase, sc_time &t) override {return tlm::TLM_UPDATED;};

  tlm::tlm_generic_payload *pending_transaction;

  sc_time timing_annotation;

  SC_HAS_PROCESS(watertank_transactor);

  explicit watertank_transactor(sc_module_name name);

  //RTL interface
  sc_in<double> input_water;
  wtank_struct wtank_packet;
  tlm::tlm_target_socket<> target_socket;

};

#endif
