#ifndef MATINV_ADJ_H
#define MATINV_ADJ_H

#include "define.h"

SC_MODULE (matinv_lu) {
    
    public:
    
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in< DATA_TYPE > indata;
    sc_out< DATA_TYPE > outdata;
    

    void LU(DATA_TYPE D[4][4]);
    void inverse(DATA_TYPE A[4][4], DATA_TYPE inverse[4][4]);
    void inv_main(void);
    
    SC_CTOR (matinv_lu) {
        
        SC_CTHREAD (inv_main, clk.pos() );
        reset_signal_is(rst, false);
        
    }
    
    ~matinv_lu(){};
    
};


#endif