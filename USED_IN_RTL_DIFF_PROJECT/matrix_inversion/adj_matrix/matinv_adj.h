#ifndef MATINV_ADJ_H
#define MATINV_ADJ_H

#include "define.h"

SC_MODULE (matinv_adj) {
    
    public:
    
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in< DATA_TYPE > indata;
    sc_out< DATA_TYPE > outdata;
    
    void getCofactor(DATA_TYPE A[4][4], DATA_TYPE temp[4][4], int p, int q, int n) ;
    DET_TYPE determinant4(DATA_TYPE A[4][4]) ;
    DET_TYPE determinant3(DATA_TYPE A[4][4]) ;
    DET_TYPE determinant2(DATA_TYPE A[4][4]) ;
    DET_TYPE determinant1(DATA_TYPE A[4][4]) ;
    void adjoint(DATA_TYPE A[4][4],DATA_TYPE adj[4][4]);
    void inverse(DATA_TYPE A[4][4], DATA_TYPE inverse[4][4]);
    void inv_main(void);
    
    SC_CTOR (matinv_adj) {
        
        SC_CTHREAD (inv_main, clk.pos() );
        reset_signal_is(rst, false);
        
    }
    
    ~matinv_adj(){}
    
};


#endif