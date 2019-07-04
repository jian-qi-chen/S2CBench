
#ifndef TEST_QSORT
#define TEST_QSORT

#include "define.h"

SC_MODULE (test_qsort){

  // Inputs
  sc_in<bool>          clk;
  sc_in<bool>          rst;

  sc_in<sc_uint<8> > odata[SIZE]; 


  //  Outputs
  sc_out<sc_uint<8> >  indata[SIZE];


  //For data feeding
  FILE * in_file, *out_golden_file, *out_file_read;
  FILE  *out_file, *diff_file;


  /* C */
  void compare_results();

  /* R */
  void recv();

  /* S */
  void send();



  SC_CTOR ( test_qsort ) {
 
    SC_CTHREAD(send,clk.pos());
    reset_signal_is(rst,false);

    SC_CTHREAD(recv,clk.pos());
    reset_signal_is(rst,false);
  }

  ~test_qsort(){}

};

#endif //TEST_QSORT
