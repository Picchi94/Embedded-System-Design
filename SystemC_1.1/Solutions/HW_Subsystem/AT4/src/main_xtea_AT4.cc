#include "xtea_AT4.hh"
#include "xtea_AT4_testbench.hh"

class XTEA_TLM_AT4_MAIN : public sc_module {

private:
  XTEA_TLM_AT4 s_target;
  xtea_AT4_testbench s_initiator;

public:

  XTEA_TLM_AT4_MAIN(sc_module_name name) : sc_module(name), s_target("target"), s_initiator("initiator") {
    s_initiator.initiator_socket(s_target.target_socket);
  }

};

int sc_main(int argc, char **argv){
  XTEA_TLM_AT4_MAIN AT4("AT4");
  sc_start();
  return 0;
}
