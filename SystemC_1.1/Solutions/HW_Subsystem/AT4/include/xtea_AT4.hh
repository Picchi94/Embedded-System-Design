#ifndef xtea_AT4_H
#define xtea_AT4_H

#include <systemc.h>
#include <tlm.h>

struct iostruct {
  sc_uint<1> mode;
  sc_uint<32> key0;
  sc_uint<32> key1;
  sc_uint<32> key2;
  sc_uint<32> key3;
  sc_uint<32> word0;
  sc_uint<32> word1;
  sc_uint<32> result0;
  sc_uint<32> result1;
};

class XTEA_TLM_AT4 : sc_module, tlm::tlm_fw_transport_if<> {
public:

  tlm::tlm_target_socket<> target_socket;

  void b_transport(tlm::tlm_generic_payload &trans, sc_time &t) ;

  bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans,
                          tlm::tlm_dmi &dmi_data) ;
  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &trans,
                                     tlm::tlm_phase &phase,
                                     sc_time &t) ;
  unsigned int transport_dbg(tlm::tlm_generic_payload &trans) ;

  iostruct ioDataStruct;
  tlm::tlm_generic_payload *pending_transaction;

  sc_event io_event;

  void IOPROCESS();

  void xtea();

  SC_HAS_PROCESS(XTEA_TLM_AT4);
  explicit XTEA_TLM_AT4(sc_module_name name_);
};

#endif
