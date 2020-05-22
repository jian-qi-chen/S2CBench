#ifndef TB_BABYLONIAN_H_
#define TB_BABYLONIAN_H_
#include "define.h"

SC_MODULE (test_babylonian){
    
    sc_in<bool> clk;
    sc_in<bool> rst;
    sc_out<sc_ufixed<32,16,SC_RND,SC_SAT> > in_num; 
    
    sc_in<sc_ufixed<32,16,SC_RND,SC_SAT> > out_num;
    
    FILE *in_file, *out_file;
    
    void recv();
    void send();

    SC_CTOR( test_babylonian){
        SC_CTHREAD(send, clk.pos());
        reset_signal_is(rst,false);

        SC_CTHREAD(recv,clk.pos());
        reset_signal_is(rst,false);
    }
    
    ~test_babylonian(){}

};

#endif
