

#ifndef DFT_H_
#define DFT_H_


#include "define.h"



SC_MODULE (dft) {
  public:
// Inputs
  sc_in_clk clk;
  sc_in<bool> rst;   

  sc_in<sc_fixed<32,16, SC_RND, SC_SAT> > in_real;   
  sc_in<sc_fixed<32,16, SC_RND, SC_SAT> > in_imag;   
  sc_in<bool> data_valid; 
  sc_in<bool> data_ack;   

// Outputs
  sc_out<sc_fixed<32,16, SC_RND, SC_SAT> > out_real;        
  sc_out<sc_fixed<32,16, SC_RND, SC_SAT> > out_imag;        
  sc_out<bool> data_req;         
  sc_out<bool> data_ready;     
  

 
  SC_CTOR(dft){

      SC_CTHREAD(entry, clk.pos() );
      reset_signal_is(rst, false);

    };

// Destructor
  ~dft(){};  


void entry(void);


};      

#endif 
