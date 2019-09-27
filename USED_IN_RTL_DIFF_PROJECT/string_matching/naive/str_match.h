#ifndef RABIN_KARP_H_
#define RABIN_KARP_H_
#include "define.h"

SC_MODULE (str_match) {
    
    public:
    
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in< sc_uint<8> > pat_in;
    sc_out< sc_int<16> > index_out;
    
    void search(unsigned char pat[], sc_uint<8> M, sc_int<16> index_list[]);
    void rk_main(void);
    
    SC_CTOR (str_match) {
        
        SC_CTHREAD(rk_main, clk.pos() );
        reset_signal_is(rst, false);
        
    }
    
    ~str_match(){};
    
};

#endif