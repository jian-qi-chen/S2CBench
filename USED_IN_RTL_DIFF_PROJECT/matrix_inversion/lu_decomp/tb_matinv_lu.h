#ifndef TB_MATINV_ADJ_H_
#define TB_MATINV_ADJ_H_

#include "define.h"
#include "stdio.h"

SC_MODULE (tb_matinv_lu){
    
    sc_in_clk clk;
    sc_in<bool> rst;
    
    sc_out< DATA_TYPE > indata;
    sc_in< DATA_TYPE > outdata;
    
    FILE *in_data_file, *out_data_file;
    
    void recv(void);
    void send(void);
    
    SC_CTOR( tb_matinv_lu ){
        SC_CTHREAD(send, clk.pos());
        reset_signal_is(rst,false);
        
        SC_CTHREAD(recv,clk.pos());
        reset_signal_is(rst,false);
    }
    
    ~tb_matinv_lu(){}

};


#endif