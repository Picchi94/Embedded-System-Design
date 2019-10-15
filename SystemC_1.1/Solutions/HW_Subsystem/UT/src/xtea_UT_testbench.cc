#include "xtea_UT_testbench.hh"
#include "xtea_UT.hh"

void XTEA_TLM_UT_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range) {

}

tlm::tlm_sync_enum XTEA_TLM_UT_testbench::nb_transport_bw(tlm::tlm_generic_payload &trans,
                                                          tlm::tlm_phase &phase,
                                                          sc_time &t) {
  return tlm::TLM_COMPLETED;
}

void XTEA_TLM_UT_testbench::run() {
  sc_time local_time;

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

  payload.set_data_ptr((unsigned char *) &xtea_packet);
  payload.set_address(0);
  payload.set_write();

  // start write transaction
  initiator_socket->b_transport(payload, local_time);

  // start read transaction
  payload.set_read();

  initiator_socket->b_transport(payload, local_time);
  if (payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
    cout << "First invocation: \n";
    cout << "   - the encryption of " << std::hex << word0 << " and " << std::hex << word1 << "\n";
    cout << "   - with key " << std::hex << key0 << key1 << key2 << key3 << "\n";
    cout << "is: " << xtea_packet.result0 << ", " << xtea_packet.result1 <<  "\n\n";
    if((xtea_packet.result0 != 0x99bbb92b) || (xtea_packet.result1 != 0x3ebd1644))
      printf("Wrong result!\n");

    printf("Second invocation: \n");
  }

  // Set to decrypt mode
  xtea_packet.mode = 1;
  xtea_packet.word0 = xtea_packet.result0;
  xtea_packet.word1 = xtea_packet.result1;

  payload.set_data_ptr((unsigned char *) &xtea_packet);
  payload.set_address(0);
  payload.set_write();

  // start write transaction
  initiator_socket->b_transport(payload, local_time);

  // start read transaction
  payload.set_read();

  initiator_socket->b_transport(payload, local_time);
  if (payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
    cout << "   - the decryption of " << word0 << " and " << word1 << "\n";
    cout << "   - with key " << std::hex << key0 << key1 << key2 << key3 << "\n";
    cout << "is: " << xtea_packet.result0 << ", " << xtea_packet.result1 <<  "\n\n";

    if((xtea_packet.result0 != 0x12345678) || (xtea_packet.result1 != 0x9abcdeff))
      printf("Wrong result!");

    printf("Done!!\n");
  }

  sc_stop();
}

XTEA_TLM_UT_testbench::XTEA_TLM_UT_testbench(sc_module_name name) : sc_module(name) {
  initiator_socket(*this);

  SC_THREAD(run);
}
