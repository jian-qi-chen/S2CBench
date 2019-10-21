//========================================================================================
// 
// File Name    : main.cpp
// Description  : Convolutional neural network header
// Release Date : 10/11/2019
// Author       : Maurice Peemen, Dongrui She 
//                Jianqi Chen, Benjamin Carrion Schafer
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Version    Author                Description
//---------------------------------------------------------------------------------------
// 2013         1.0       M. Peemen, D. She     original pure C implementation          
// 10/11/2019   1.1       Jianqi Chen           convert into synthesizable systemC
//=======================================================================================

#ifndef CNN_H_
#define CNN_H_
#define SC_INCLUDE_FX
#include "systemc.h"

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
                            const sc_fixed<18,12,SC_RND,SC_SAT> bias[], const sc_fixed<12,8,SC_RND,SC_SAT> weight[]);
    int run_convolution_layer4(unsigned char in_layer[], const sc_fixed<18,12,SC_RND,SC_SAT> bias[],
                            const sc_fixed<12,8,SC_RND,SC_SAT> weight[], unsigned int detect[]);
    sc_fixed<22,16,SC_RND,SC_SAT> activation_function( sc_fixed<22,16,SC_RND,SC_SAT> sum);

    SC_CTOR (cnn) {
        SC_CTHREAD (cnn_main, clk.pos() );
        reset_signal_is(rst, false);
        sensitive << clk.pos();
        
    }

};

#endif