#ifndef xtea_RTL_H
#define xtea_RTL_H

#include "systemc.h"

class xtea_RTL : public sc_core::sc_module {

    public:
      sc_in<sc_uint<1> > input_rdy;
      sc_in<sc_uint<32> > word1;
      sc_in<sc_uint<32> > word2;
      sc_in<sc_uint<32> > key0;
      sc_in<sc_uint<32> > key1;
      sc_in<sc_uint<32> > key2;
      sc_in<sc_uint<32> > key3;
      sc_in<sc_uint<1> > mode;

      sc_out<sc_uint<32> > result0;
      sc_out<sc_uint<32> > result1;
      sc_out<sc_uint<1> > output_rdy;

      sc_in<bool>         rst;
      sc_in_clk clk;



      typedef enum{Reset_ST, ST_0, ST_1, ST_2, ST_3, ST_4, ST_5, ST_6, ST_7, ST_8, ST_9, ST_10, Final_ST} STATES;
      sc_signal<STATES> STATUS, NEXT_STATUS;
      /*
      SEGNALI DA DEFINIRE
      */
      sc_signal<sc_uint<6> > counter;
      sc_signal<sc_uint<32> > delta;
      sc_signal<sc_uint<32> > v0;
      sc_signal<sc_uint<32> > v1;
      sc_signal<sc_uint<64> > sum;
      sc_signal<sc_uint<32> > temp0;

      void fsm(void);
      void datapath(void);

      SC_CTOR(xtea_RTL){
        SC_METHOD(datapath);
        sensitive << rst.neg();
        sensitive << clk.pos();

        SC_METHOD(fsm);
        sensitive << STATUS << input_rdy;
    };
};

#endif
