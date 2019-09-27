#ifndef CNN_H_
#define CNN_H_
#define SC_INCLUDE_FX
#include "systemc.h"

# define WEIGHT3_DATA_TYPE float//sc_fixed<12,8,SC_RND,SC_SAT>
# define Y_DATA_TYPE sc_fixed<22,16,SC_RND,SC_SAT>
# define E_DATA_TYPE sc_fixed<24,8,SC_RND,SC_SAT>

SC_MODULE (cnn) {
    public:
    
    sc_in_clk clk;
    sc_in<bool> rst;
    
    sc_in< sc_uint<8> > indata;
    sc_out< sc_uint<16> > outdata;
    sc_out< sc_uint<8> > detection_out;

    void cnn_main(void);
    void run_convolution_layer1(unsigned char in_layer[], unsigned char out_layer[],
                            const sc_fixed<18,12,SC_RND,SC_SAT> bias[], const sc_fixed<12,8,SC_RND,SC_SAT> weight[]);
    void run_convolution_layer2(unsigned char in_layer[], unsigned char out_layer[],
                            const sc_fixed<18,12,SC_RND,SC_SAT> bias[], const sc_fixed<12,8,SC_RND,SC_SAT> weight[]);
    void run_convolution_layer3(unsigned char in_layer[], unsigned char out_layer[],
                            const sc_fixed<18,12,SC_RND,SC_SAT> bias[], const WEIGHT3_DATA_TYPE weight[]);
    int run_convolution_layer4(unsigned char in_layer[], const sc_fixed<18,12,SC_RND,SC_SAT> bias[],
                            const sc_fixed<12,8,SC_RND,SC_SAT> weight[], unsigned int detect[]);
    Y_DATA_TYPE activation_function( Y_DATA_TYPE sum);

    SC_CTOR (cnn) {
        SC_CTHREAD (cnn_main, clk.pos() );
        reset_signal_is(rst, false);
        sensitive << clk.pos();
        
    }

};

#endif