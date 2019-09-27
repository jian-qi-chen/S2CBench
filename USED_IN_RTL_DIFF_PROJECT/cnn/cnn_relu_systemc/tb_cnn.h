#ifndef TB_CNN_H_
#define TB_CNN_H_
#include "systemc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 100  //Line buffer size for read write 

SC_MODULE (tb_cnn){
    
    public:
    
    sc_in_clk clk;
    sc_in<bool> rst;
    
    sc_out< sc_uint<8> >  indata;
    sc_in< sc_uint<16> > outdata;
    sc_in< sc_uint<8> > detection_out;
    
    void read_image_pgm(unsigned char image[], char filename[], int imageWidth, int imageHeight);
    
    void write_image_pgm(unsigned char image[], const char filename[], int imageWidth, int imageHeight);
    
    void annotate_img(unsigned char img[], unsigned int detectarray[], int detections);
    
    void recv(void);
    
    void send(void);
    
    SC_CTOR( tb_cnn ){
        
        SC_CTHREAD( send, clk.pos() );
        reset_signal_is(rst, false);
        
        SC_CTHREAD( recv, clk.pos() );
        reset_signal_is(rst, false);
        
    }
    
    ~tb_cnn(){};
    
};



#endif