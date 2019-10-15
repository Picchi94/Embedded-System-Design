#include "xtea_LT.hh"

XTEA_TLM_LT::XTEA_TLM_LT(sc_module_name name_)
    : sc_module(name_), target_socket("target_socket"), pending_transaction(NULL) {
  target_socket(*this);
}

void XTEA_TLM_LT::b_transport(tlm::tlm_generic_payload &trans, sc_time &t) {
  timing_annotation=SC_ZERO_TIME;
  ioDataStruct = *((iostruct *) trans.get_data_ptr());

  if (trans.is_write()) {
    xtea();
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    *((iostruct *) trans.get_data_ptr()) = ioDataStruct;
  } else if (trans.is_read()) {
    ioDataStruct.result0 = result0;
    ioDataStruct.result1 = result1;
    *((iostruct *) trans.get_data_ptr()) = ioDataStruct;
  }
  t += timing_annotation;
}

bool XTEA_TLM_LT::get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data) {
  return false;
}

tlm::tlm_sync_enum XTEA_TLM_LT::nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t) {
  return tlm::TLM_COMPLETED;
}

unsigned int XTEA_TLM_LT::transport_dbg(tlm::tlm_generic_payload &trans) {
  return 0;
}

void XTEA_TLM_LT::xtea() {
  sc_uint<32> i, delta, v0, v1, temp;
  sc_uint<64> sum;
  v0 = ioDataStruct.word0;
  v1 = ioDataStruct.word1;
  sum = 0;
  result0 = 0;
  result1 = 0;

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
  result0 = v0;
  result1 = v1;

  timing_annotation += sc_time(100, SC_NS);
}
