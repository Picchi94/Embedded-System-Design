#include "xtea_RTL_testbench.hh"
#include "xtea_RTL.hh"

int sc_main(int argc, char * argv[]){

  sc_signal<bool> clock;

  sc_signal<bool> rst;
  sc_signal<sc_uint<1> > input_rdy;
  sc_signal<sc_uint<32> > word1;
  sc_signal<sc_uint<32> > word2;
  sc_signal<sc_uint<32> > key0;
  sc_signal<sc_uint<32> > key1;
  sc_signal<sc_uint<32> > key2;
  sc_signal<sc_uint<32> > key3;
  sc_signal<sc_uint<1> > mode;

  sc_signal<sc_uint<32> > result0;
  sc_signal<sc_uint<32> > result1;
  sc_signal<sc_uint<1> > output_rdy;

  xtea_RTL_testbench    tb("tb");
  xtea_RTL              xtea("xtea");


  xtea.input_rdy(input_rdy);
  xtea.word1(word1);
  xtea.word2(word2);
  xtea.key0(key0);
  xtea.key1(key1);
  xtea.key2(key2);
  xtea.key3(key3);
  xtea.mode(mode);
  xtea.result0(result0);
  xtea.result1(result1);
  xtea.output_rdy(output_rdy);
  xtea.clk(clock);
  xtea.rst(rst);

  tb.input_rdy(input_rdy);
  tb.word1(word1);
  tb.word2(word2);
  tb.key0(key0);
  tb.key1(key1);
  tb.key2(key2);
  tb.key3(key3);
  tb.mode(mode);
  tb.result0(result0);
  tb.result1(result1);
  tb.output_rdy(output_rdy);
  tb.clk(clock);
  tb.rst(rst);

  sc_start();

  return 0;
};
