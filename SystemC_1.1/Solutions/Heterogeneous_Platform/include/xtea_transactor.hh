#ifndef XTEA_TRANSACTOR
#define XTEA_TRANSACTOR

#include <systemc.h>
#include <tlm.h>
#include <struct.hh>

class xtea_transactor : public sc_module, public tlm::tlm_fw_transport_if<> {
public:
  //TLM side
  tlm::tlm_target_socket<> target_socket;
  xtea_struct xtea_packet;
  sc_event begin_write, end_write;

  //TLM Interface
  virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &t) override;
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data) override;
  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t) override;
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload &trans) override;

  //RTL ports
  sc_out<sc_uint<1> > input_rdy;
  sc_out<sc_uint<32> > word1;
  sc_out<sc_uint<32> > word2;
  sc_out<sc_uint<32> > key0;
  sc_out<sc_uint<32> > key1;
  sc_out<sc_uint<32> > key2;
  sc_out<sc_uint<32> > key3;
  sc_out<sc_uint<1> > mode;

  sc_in<sc_uint<1> > output_rdy;

  sc_out<bool>         rst;
  sc_in_clk clk;

  void WRITEPROCESS();

  void end_of_elaboration();

  SC_HAS_PROCESS(xtea_transactor);
  xtea_transactor(sc_module_name name_);
};

#endif
