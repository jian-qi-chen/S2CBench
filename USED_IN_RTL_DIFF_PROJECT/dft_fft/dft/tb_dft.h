#ifndef TEST_DFT
#define TEST_DFT
 
#include "define.h"

SC_MODULE (test_dft){

  // Inputs
  sc_in<bool>          clk;
  sc_in<bool>          rst;

  sc_in<sc_fixed<32,16, SC_RND, SC_SAT> > out_real;        
  sc_in<sc_fixed<32,16, SC_RND, SC_SAT> > out_imag;        
  sc_in<bool> data_req;         
  sc_in<bool> data_ready; 


  //  Outputs
  sc_out<sc_fixed<32,16, SC_RND, SC_SAT> > in_real;   
  sc_out<sc_fixed<32,16, SC_RND, SC_SAT> > in_imag;   
  sc_out<bool> data_valid; 
  sc_out<bool> data_ack;   



  //For data feeding
  FILE * in_file, *out_file_golden, *out_file, *diff_file;


  /* C */
  void compare_results();

  /* R */
  void recv();

  /* S */
  void send();



  SC_CTOR ( test_dft ) {
 
    SC_CTHREAD(send,clk.pos());
    reset_signal_is(rst,false);

    SC_CTHREAD(recv,clk.pos());
    reset_signal_is(rst,false);
  };

  ~test_dft(){};

};

#endif //TEST_FFT
