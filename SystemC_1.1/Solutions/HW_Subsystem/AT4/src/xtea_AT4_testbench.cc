#include "xtea_AT4_testbench.hh"
#include "xtea_AT4.hh"

void xtea_AT4_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{

}

tlm::tlm_sync_enum xtea_AT4_testbench::nb_transport_bw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t)
{

  if (!response_pending) {
    cout << "[ERROR] Unexpected state" << endl;
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  if (phase != tlm::BEGIN_RESP) {
    cout << "[ERROR] Unexpected phase" << endl;
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  available_response.notify();
  phase = tlm::END_RESP;

  return tlm::TLM_COMPLETED;
}

void xtea_AT4_testbench::run() {
  sc_time local_time = SC_ZERO_TIME;

  iostruct xtea_packet;
  tlm::tlm_generic_payload payload;
  sc_uint<32> word0 = 0x12345678;
  sc_uint<32> word1 = 0x9abcdeff;
  sc_uint<32> key0 = 0x6a1d78c8;
  sc_uint<32> key1 = 0x8c86d67f;
  sc_uint<32> key2 = 0x2a65bfbe;
  sc_uint<32> key3 = 0xb4bd6e46;

  // Set to encrypt mode
  xtea_packet.mode = 0;
  xtea_packet.key0 = key0;
  xtea_packet.key1 = key1;
  xtea_packet.key2 = key2;
  xtea_packet.key3 = key3;
  xtea_packet.word0 = word0;
  xtea_packet.word1 = word1;

  // set phase: begin request
  tlm::tlm_phase phase = tlm::BEGIN_REQ;

  // update payload
  payload.set_address(0);
  payload.set_data_ptr((unsigned char *) &xtea_packet);
  payload.set_write();

  cout << "\nInvoking the nb_transport_fw primitive, writing.." << endl;
  tlm::tlm_sync_enum result = initiator_socket->nb_transport_fw(payload, phase,
                                                                local_time);

  if (result == tlm::TLM_COMPLETED) {

    cout << "[ERROR] Transaction error. Forcing simulation stop." << endl;
    sc_stop();
  }

  if (phase != tlm::END_REQ) {
    cout << "[ERROR] Unexpected protocol phase. Forcing simulation stop." << endl;
    sc_stop();
  }

  cout << "Waiting nb_transport_bw " << endl;
  response_pending = true;
  wait(available_response);

  response_pending = false;

  phase = tlm::BEGIN_REQ;
  payload.set_address(0);
  payload.set_data_ptr((unsigned char *) &xtea_packet);
  payload.set_read();

  cout << "\nInvoking the nb_transport_fw primitive, writing.." << endl;
  result = initiator_socket->nb_transport_fw(payload, phase, local_time);

  if (result == tlm::TLM_COMPLETED) {
    cout << "[ERROR] Transaction error. Forcing simulation stop." << endl;
    sc_stop();
  }

  if (phase != tlm::END_REQ) {
    cout << "[ERROR] Unexpected protocol phase. Forcing simulation stop." << endl;
    sc_stop();
  }

  cout << "Waiting nb_transport_bw " << endl;
  response_pending = true;
  wait(available_response);

  // target invoke the bk and the AT4 protocol is finished
  response_pending = false;

  if (payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
    cout << "TLM-AT4 protocol is correct" << endl;
    cout << "\nFirst invocation: \n";
    cout << "   - the encryption of " << std::hex << word0 << " and " << std::hex << word1 << "\n";
    cout << "   - with key " << std::hex << key0 << key1 << key2 << key3 << "\n";
    cout << "is: " << xtea_packet.result0 << ", " << xtea_packet.result1 <<  "\n\n";
    if((xtea_packet.result0 != 0x99bbb92b) || (xtea_packet.result1 != 0x3ebd1644))
      printf("Wrong result!\n");  }

  // Set to decrypt mode
  xtea_packet.mode = 1;
  xtea_packet.word0 = xtea_packet.result0;
  xtea_packet.word1 = xtea_packet.result1;

  // begin request
  phase = tlm::BEGIN_REQ;

  payload.set_data_ptr((unsigned char *) &xtea_packet);
  payload.set_address(0);
  payload.set_write();

  cout << "Invoking the nb_transport_fw primitive, writing.." << endl;
  result = initiator_socket->nb_transport_fw(payload, phase, local_time);

  if (result == tlm::TLM_COMPLETED) {

    cout << "[ERROR] Transaction error. Forcing simulation stop." << endl;
    sc_stop();
  }

  if (phase != tlm::END_REQ) {
    cout << "[ERROR] Unexpected protocol phase. Forcing simulation stop." << endl;
    sc_stop();
  }

  cout << "Waiting nb_transport_bw" << endl;
  response_pending = true;
  wait(available_response);


  response_pending = false;

  phase = tlm::BEGIN_REQ;
  payload.set_address(0);
  payload.set_data_ptr((unsigned char *) &xtea_packet);
  payload.set_read();

  cout << "\nInvoking the nb_transport_fw primitive, reading.." << endl;
  result = initiator_socket->nb_transport_fw(payload, phase, local_time);

  if (result == tlm::TLM_COMPLETED) {
    cout << "[ERROR] Transaction error. Forcing simulation stop." << endl;
    sc_stop();
  }

  if (phase != tlm::END_REQ) {
    cout << "[ERROR] Unexpected protocol phase. Forcing simulation stop." << endl;
    sc_stop();
  }

  cout << "Waiting nb_transport_bw " << endl;
  response_pending = true;
  wait(available_response);

  response_pending = false;

  if (payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
    cout << "TLM-AT4 protocol is correct" << endl;
    printf("\nSecond invocation: \n");
    cout << "   - the decryption of " << word0 << " and " << word1 << "\n";
    cout << "   - with key " << std::hex << key0 << key1 << key2 << key3 << "\n";
    cout << "is: " << xtea_packet.result0 << ", " << xtea_packet.result1 <<  "\n\n";

    if((xtea_packet.result0 != 0x12345678) || (xtea_packet.result1 != 0x9abcdeff))
      printf("Wrong result!");

    printf("Done!!\n");  }

  sc_stop();
}

xtea_AT4_testbench::xtea_AT4_testbench(sc_module_name name)
  : sc_module(name)
  , response_pending(false)
{

  initiator_socket(*this);

  SC_THREAD(run);
}
