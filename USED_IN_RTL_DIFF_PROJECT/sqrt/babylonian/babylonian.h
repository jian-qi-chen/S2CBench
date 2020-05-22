#ifndef BABYLONIAN_H_
#define BABYLONIAN_H_
#include "define.h"

SC_MODULE (babylonian){

    public:
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<sc_ufixed<32,16,SC_RND,SC_SAT> > in_num; 
    
    sc_out<sc_ufixed<32,16,SC_RND,SC_SAT> > out_num;
    
    void babylonian_main(void);
    
    SC_CTOR(babylonian){
        SC_CTHREAD (babylonian_main, clk.pos() );
        reset_signal_is(rst, false);
        sensitive << clk.pos();
    }
    
    ~babylonian(){}
    
};

#endif
