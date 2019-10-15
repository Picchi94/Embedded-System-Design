#include "xtea_AT4.hh"

XTEA_TLM_AT4::XTEA_TLM_AT4(sc_module_name name_)
    : sc_module(name_), target_socket("target_socket"), pending_transaction(NULL) {
  target_socket(*this);
  SC_THREAD(IOPROCESS);

}

void XTEA_TLM_AT4::b_transport(tlm::tlm_generic_payload &trans, sc_time &t) {

}

bool XTEA_TLM_AT4::get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data) {
  return false;
}

tlm::tlm_sync_enum XTEA_TLM_AT4::nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t) {


  if (pending_transaction != NULL) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }


  if (phase != tlm::BEGIN_REQ) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  pending_transaction = &trans;
  ioDataStruct        = *((iostruct *) trans.get_data_ptr());

  phase = tlm::END_REQ;

  io_event.notify();

  return tlm::TLM_UPDATED;
}

unsigned int XTEA_TLM_AT4::transport_dbg(tlm::tlm_generic_payload &trans) {
  return 0;
}

void XTEA_TLM_AT4::IOPROCESS() {

  sc_time timing_annotation;

  while (true) {
    wait(io_event);

    wait(100, SC_NS);

    if (pending_transaction->is_write()) {
      xtea();
    }
    pending_transaction->set_response_status(tlm::TLM_OK_RESPONSE);

    *((iostruct *) pending_transaction->get_data_ptr()) = ioDataStruct;

    tlm::tlm_phase phase = tlm::BEGIN_RESP;
    target_socket->nb_transport_bw(*pending_transaction, phase, timing_annotation);

    pending_transaction = NULL;
  }
}

void XTEA_TLM_AT4::xtea() {
  sc_uint<32> i, delta, v0, v1, temp;
  sc_uint<64> sum;
  v0 = ioDataStruct.word0;
  v1 = ioDataStruct.word1;
  sum = 0;

  if (!ioDataStruct.mode) {
    // encipher
    delta = 0x9e3779b9;
    for (i = 0; i < 32; i++) {

      if ((sum & 3) == 0)
        temp = ioDataStruct.key0;
      else if ((sum & 3) == 1)
        temp = ioDataStruct.key1;
      else if ((sum & 3) == 2)
        temp = ioDataStruct.key2;
      else
        temp = ioDataStruct.key3;

      v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);

      sum += delta;

      if (((sum >> 11) & 3) == 0)
        temp = ioDataStruct.key0;
      else if (((sum >> 11) & 3) == 1)
        temp = ioDataStruct.key1;
      else if (((sum >> 11) & 3) == 2)
        temp = ioDataStruct.key2;
      else
        temp = ioDataStruct.key3;

      v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);
    }
  } else if (ioDataStruct.mode) {
    // decipher
    delta = 0x9e3779b9;
    sum = delta * 32;
    for (i = 0; i < 32; i++) {

      if (((sum >> 11) & 3) == 0)
        temp = ioDataStruct.key0;
      else if (((sum >> 11) & 3) == 1)
        temp = ioDataStruct.key1;
      else if (((sum >> 11) & 3) == 2)
        temp = ioDataStruct.key2;
      else
        temp = ioDataStruct.key3;

      v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);

      sum -= delta;

      if ((sum & 3) == 0)
        temp = ioDataStruct.key0;
      else if ((sum & 3) == 1)
        temp = ioDataStruct.key1;
      else if ((sum & 3) == 2)
        temp = ioDataStruct.key2;
      else
        temp = ioDataStruct.key3;

      v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);
    }
  }
  ioDataStruct.result0 = v0;
  ioDataStruct.result1 = v1;

}
