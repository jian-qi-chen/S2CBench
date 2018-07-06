#ifndef JPEG_ENCODER_H_
#define JPEG_ENCODER_H_

#include "define.h"

SC_MODULE (jpeg_encoder){

  public:

    sc_in_clk clk; //clock
    sc_in<bool> rst; //reset

    //inputs
    sc_in <sc_uint<8> > input_col[8];
   
    //outputs
    sc_out <sc_uint<1> > output[64];
    sc_out <sc_uint<9> > last;
    
    // Variables declarations
    sc_uint<8>  line_buffer[8][8];
    sc_int<DCT_OUT_WIDTH>  dct_output[8][8];
    sc_int<QUAN_OUT_WIDTH>  quantization_output[8][8];
    sc_int<QUAN_OUT_WIDTH>  previous_dc;
    sc_uint<1> encode_output[512];
    sc_uint<9> last_bit;
    sc_uint<1> check_byte[8];
    sc_uint<3> check_index;

    //function prototypes
    
    void jpeg_quantization(sc_int<DCT_OUT_WIDTH> [8][8]);
    void jpeg_dct(sc_uint<8> [8][8]);
    void jpeg_main(void);
    void jpeg_zigzag(sc_int<QUAN_OUT_WIDTH> input[8][8],sc_int<QUAN_OUT_WIDTH> zz[64]);
    int jpeg_rle(sc_int<QUAN_OUT_WIDTH> zz[64], sc_int<QUAN_OUT_WIDTH> rl[64]);
    int jpeg_getCat(int a);
    void jpeg_DCcode(sc_int<QUAN_OUT_WIDTH> rl0);
    void jpeg_ACcode(sc_int<QUAN_OUT_WIDTH> rl[64],int length);
    void jpeg_addzero(void);

    SC_CTOR (jpeg_encoder){
      SC_CTHREAD(jpeg_main,clk.pos());
      reset_signal_is(rst,false);
    };

    ~jpeg_encoder(){};

};

#endif
