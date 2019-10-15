#include "xtea_LT.hh"
#include "xtea_LT_testbench.hh"

class XTEA_TLM_LT_MAIN : public sc_module {

private:
  XTEA_TLM_LT s_target;
  XTEA_TLM_LT_testbench s_initiator;

public:

  XTEA_TLM_LT_MAIN(sc_module_name name) : sc_module(name), s_target("target"), s_initiator("initiator") {
    s_initiator.initiator_socket(s_target.target_socket);
  }

};

int sc_main(int argc, char **argv){
  XTEA_TLM_LT_MAIN LT("LT");
  sc_start();
  return 0;
}
