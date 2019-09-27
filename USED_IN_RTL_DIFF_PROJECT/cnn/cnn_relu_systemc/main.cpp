#include "cnn.h"
#include "tb_cnn.h"

int sc_main(int argc, char* argv[]){
  sc_clock                clk("clk", 25, SC_NS, 0.5, 0, SC_NS, true);
  sc_signal<bool> rst;
  
  sc_signal< sc_uint<8> > indata;
  sc_signal< sc_uint<16> > outdata;
  sc_signal< sc_uint<8> > detection_out;
  
  cnn cnn_obj("cnn");
  tb_cnn tb_obj("tb_cnn");
  
  // connections
  cnn_obj.clk(clk);
  cnn_obj.rst(rst);
  cnn_obj.indata(indata);
  cnn_obj.outdata(outdata);
  cnn_obj.detection_out(detection_out);
  
  tb_obj.clk(clk);
  tb_obj.rst(rst);
  tb_obj.indata(indata);
  tb_obj.outdata(outdata);
  tb_obj.detection_out(detection_out);
  
  sc_start(25, SC_NS);
  rst.write(0);
  
  sc_start(25, SC_NS);
  rst.write(1);
  
  sc_start();
  
  return 0;
}