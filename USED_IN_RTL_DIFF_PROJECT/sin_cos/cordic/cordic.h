#ifndef CORDIC_H_
#define CORDIC_H_
#include "define.h"

SC_MODULE (cordic){

    public:
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<sc_int<16> > in_angle; // in_angle = radian * MUL (MUL is defined in define.h)
    
    sc_out<sc_int<16> > out_sin; // out_sin = sin(in) * MUL
    sc_out<sc_int<16> > out_cos; // out_cos = cos(in) * MUL
    
    void cordic_main(void);
    
    SC_CTOR(cordic){
        SC_CTHREAD (cordic_main, clk.pos() );
        reset_signal_is(rst, false);
        sensitive << clk.pos();
    }
    
    ~cordic(){}
    
};

#endif
