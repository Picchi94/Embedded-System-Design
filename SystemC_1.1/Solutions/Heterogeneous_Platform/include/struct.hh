#ifndef STRUCT_HH
#define STRUCT_HH

#define KEY0 0x6a1d78c8
#define KEY1 0x8c86d67f
#define KEY2 0x2a65bfbe
#define KEY3 0xb4bd6e46

struct wtank_struct {
  double input_water;
};

struct xtea_struct {
  sc_uint<1> mode;

  sc_uint<32> key0;
  sc_uint<32> key1;
  sc_uint<32> key2;
  sc_uint<32> key3;

  sc_uint<32> word1;
  sc_uint<32> word2;

  sc_uint<32> result1;
  sc_uint<32> result2;
};

#endif
