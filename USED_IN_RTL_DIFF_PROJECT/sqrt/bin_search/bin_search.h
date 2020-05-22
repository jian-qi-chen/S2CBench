#ifndef BIN_SEARCH_H_
#define BIN_SEARCH_H_
#include "define.h"

SC_MODULE (bin_search){

    public:
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<sc_ufixed<32,16,SC_RND,SC_SAT> > in_num; 
    
    sc_out<sc_ufixed<32,16,SC_RND,SC_SAT> > out_num;
    
    void bin_search_main(void);
    
    SC_CTOR(bin_search){
        SC_CTHREAD (bin_search_main, clk.pos() );
        reset_signal_is(rst, false);
        sensitive << clk.pos();
    }
    
    ~bin_search(){}
    
};

#endif
