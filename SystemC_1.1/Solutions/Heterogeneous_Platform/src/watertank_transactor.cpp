#include "watertank_transactor.hh"

watertank_transactor::watertank_transactor(sc_module_name name)
    : sc_module(name), target_socket("target_socket") {
      target_socket(*this);
 }

void watertank_transactor::b_transport(tlm::tlm_generic_payload &trans, sc_time &t) {
  if(trans.is_read()){
  wtank_packet = *((wtank_struct *) trans.get_data_ptr());

  transduce();

  *((wtank_struct *) trans.get_data_ptr()) = wtank_packet;

  } else {
          std::cout << "wtank transactor: error in b_transport " << '\n';
          sc_stop();
  }
}

void watertank_transactor::transduce() {
  wtank_packet.input_water = input_water.read();
}

void watertank_transactor::end_of_elaboration() {
  sc_module::end_of_elaboration();
}
