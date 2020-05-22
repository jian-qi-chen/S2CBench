#ifndef TB_CORDIC_H_
#define TB_CORDIC_H_
#include "define.h"

SC_MODULE (test_cordic){
    
    sc_in<bool> clk;
    sc_in<bool> rst;
    sc_in<sc_int<16> > out_sin;
    sc_in<sc_int<16> > out_cos;
    
    sc_out<sc_int<16> > in_angle;
    
    FILE *in_file, *out_file;
    
    void recv();
    void send();

    SC_CTOR( test_cordic){
        SC_CTHREAD(send, clk.pos());
        reset_signal_is(rst,false);

        SC_CTHREAD(recv,clk.pos());
        reset_signal_is(rst,false);
    }
    
    ~test_cordic(){}

};

#endif
