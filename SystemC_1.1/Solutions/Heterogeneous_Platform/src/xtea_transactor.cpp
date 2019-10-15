#include "xtea_transactor.hh"

xtea_transactor::xtea_transactor(sc_module_name name_)
    : sc_module(name_), target_socket("target_socket") {
  target_socket(*this);
  SC_THREAD(WRITEPROCESS);
  sensitive << clk.pos();
}

void xtea_transactor::b_transport(tlm::tlm_generic_payload &trans,
                                      sc_time &t) {

  if(trans.is_write()) {
    xtea_packet = *((xtea_struct *)trans.get_data_ptr());
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    begin_write.notify();
    wait(end_write);
  } else {
    std::cout << "xtea_transactor: error in b_transport" << '\n';
    sc_stop();
  }
}

void xtea_transactor::WRITEPROCESS() {
  while (true) {
    wait(begin_write);
    rst.write(true);
    wait();
    rst.write(false);
    word1.write(xtea_packet.result1);
    word2.write(xtea_packet.result2);
    key0.write(xtea_packet.key0);
    key1.write(xtea_packet.key1);
    key2.write(xtea_packet.key2);
    key3.write(xtea_packet.key3);
    mode.write(1);
    input_rdy.write(1);
    end_write.notify();
    wait();
  }
}

void xtea_transactor::end_of_elaboration() { }

bool xtea_transactor::get_direct_mem_ptr(tlm::tlm_generic_payload &trans,
                                             tlm::tlm_dmi &dmi_data) {
  return false;
}

tlm::tlm_sync_enum
xtea_transactor::nb_transport_fw(tlm::tlm_generic_payload &trans,
                                     tlm::tlm_phase &phase, sc_time &t) {
  return tlm::TLM_UPDATED;
}

unsigned int
xtea_transactor::transport_dbg(tlm::tlm_generic_payload &trans) {
  return 0;
}
