#include "controller.hh"
#include "struct.hh"

controller::controller(sc_module_name name) : sc_module(name) {
  wtank_socket(*this);
  xtea_socket(*this);
    t = 0.7;
    wl = 0;
    SC_THREAD(run);

    tlm_utils::tlm_quantumkeeper::set_global_quantum(sc_time(500, SC_NS));
    m_qk.reset();
  }

void controller::run() {
  sc_time local_time;
  tlm::tlm_generic_payload payload;

  while(true) {
    wait(1, SC_SEC);

    payload.set_data_ptr((unsigned char *) &wtank_packet.input_water);
    payload.set_address(0);
    payload.set_read();
    wtank_socket -> b_transport(payload, local_time);

    xtea_packet.mode = 0;
    xtea_packet.key0 = key0;
    xtea_packet.key1 = key1;
    xtea_packet.key2 = key2;
    xtea_packet.key3 = key3;
    xtea_packet.word2 = 0;

    if (payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
      wait(2, SC_SEC);
      wl = wtank_packet.input_water;
    }
    if (wl >= 5 && wl <= 8.8) {
      xtea_packet.word1 = STATE::IDLE;
    } else if (wl > 8.8) {
      xtea_packet.word1 = STATE::CLOSE;
      t *= 0.7;
    } else if (wl < 5) {
      xtea_packet.word1 = STATE::OPEN;
      t *= 1.1;
    }

    encrypt();

    payload.set_data_ptr((unsigned char *) &xtea_packet);
    payload.set_address(0);
    payload.set_write();

    xtea_socket -> b_transport(payload, local_time);

    if (payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
      aperture_tr.write(t);
    }

    m_qk.set(local_time);
    if(m_qk.need_sync()){
      m_qk.sync();
    }
  }
}

void controller::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range) {}

tlm::tlm_sync_enum controller::nb_transport_bw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t) {
  return tlm::TLM_COMPLETED;
}

void controller::encrypt() {
  sc_uint<32> i, delta, v0, v1, temp;
  sc_uint<64> sum;
  v0 = xtea_packet.word1;
  v1 = xtea_packet.word2;
  sum = 0;

  if (!xtea_packet.mode) {
    // crypt
    delta = 0x9e3779b9;
    for (i = 0; i < 32; i++) {

      if ((sum & 3) == 0)
        temp = xtea_packet.key0;
      else if ((sum & 3) == 1)
        temp = xtea_packet.key1;
      else if ((sum & 3) == 2)
        temp = xtea_packet.key2;
      else
        temp = xtea_packet.key3;

      v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);

      sum += delta;

      if (((sum >> 11) & 3) == 0)
        temp = xtea_packet.key0;
      else if (((sum >> 11) & 3) == 1)
        temp = xtea_packet.key1;
      else if (((sum >> 11) & 3) == 2)
        temp = xtea_packet.key2;
      else
        temp = xtea_packet.key3;

      v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);
    }
      xtea_packet.result1 = v0;
      xtea_packet.result2 = v1;

      }
  //timing_annotation += sc_time(100, SC_NS);
}
