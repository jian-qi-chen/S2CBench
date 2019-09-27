#ifndef TB_RABIN_KARP_H_
#define TB_RABIN_KARP_H_
#include <stdio.h>
#include "define.h"

SC_MODULE (tb_rabin_karp) {
    public:
    
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_out< sc_uint<8> > pat_in;
    sc_in< sc_int<16> > index_out;
    
    FILE *in_data_file, *out_data_file;
    
    void recv(void);
    void send(void);
    
    SC_CTOR( tb_rabin_karp ){
        SC_CTHREAD(send, clk.pos());
        reset_signal_is(rst,false);
        
        SC_CTHREAD(recv, clk.pos());
        reset_signal_is(rst,false);
    }

    ~tb_rabin_karp(){};
};

#endif