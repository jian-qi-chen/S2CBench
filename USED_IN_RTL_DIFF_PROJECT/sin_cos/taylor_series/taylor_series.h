#ifndef TAYLOR_H_
#define TAYLOR_H_
#include "define.h"

SC_MODULE (taylor_series){

    public:
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<D_TYPE > in_angle; // in_angle = radian * MUL (MUL is defined in define.h)
    
    sc_out<D_TYPE > out_sin; // out_sin = sin(in) * MUL
    sc_out<D_TYPE > out_cos; // out_cos = cos(in) * MUL
    
    void taylor_series_main(void);
    
    SC_CTOR(taylor_series){
        SC_CTHREAD (taylor_series_main, clk.pos() );
        reset_signal_is(rst, false);
        sensitive << clk.pos();
    }
    
    ~taylor_series(){}
    
};

#endif
