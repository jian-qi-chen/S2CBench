#ifndef TB_TAYLOR_H_
#define TB_TAYLOR_H_
#include "define.h"

SC_MODULE (test_taylor_series){
    
    sc_in<bool> clk;
    sc_in<bool> rst;
    sc_in<D_TYPE > out_sin;
    sc_in<D_TYPE > out_cos;
    
    sc_out<D_TYPE > in_angle;
    
    FILE *in_file, *out_file;
    
    void recv();
    void send();

    SC_CTOR( test_taylor_series){
        SC_CTHREAD(send, clk.pos());
        reset_signal_is(rst,false);

        SC_CTHREAD(recv,clk.pos());
        reset_signal_is(rst,false);
    }
    
    ~test_taylor_series(){}

};

#endif
