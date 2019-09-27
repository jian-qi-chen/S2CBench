#include "dft.h"
#include "tb_dft.h"


int sc_main(int argc, char** argv)
{
  sc_clock                clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
  sc_signal<bool>         rst;
 
  sc_signal<sc_fixed<32,16, SC_RND, SC_SAT> > in_real;   
  sc_signal<sc_fixed<32,16, SC_RND, SC_SAT> > in_imag;  
  sc_signal<sc_fixed<32,16, SC_RND, SC_SAT> > out_real;        
  sc_signal<sc_fixed<32,16, SC_RND, SC_SAT> > out_imag;  
  sc_signal<bool> data_valid; 
  sc_signal<bool> data_ack;   
  sc_signal<bool> data_req;         
  sc_signal<bool> data_ready;  
     

   dft u_DFT("DFT");
   test_dft test("test_DFT");

   //connect to bubble sort
   u_DFT.clk( clk );
   u_DFT.rst( rst );
   u_DFT.in_real( in_real);
   u_DFT.in_imag( in_imag);
   u_DFT.data_valid( data_valid );
   u_DFT.data_ack( data_ack );

   u_DFT.out_real( out_real);
   u_DFT.out_imag( out_imag);
   u_DFT.data_req( data_req);
   u_DFT.data_ready( data_ready);
 
  // connect to test bench
  test.clk( clk );
  test.rst( rst );
  test.in_real( in_real);
  test.in_imag( in_imag);
  test.data_valid( data_valid );
  test.data_ack( data_ack );

  test.out_real( out_real);
  test.out_imag( out_imag);
  test.data_req( data_req);
  test.data_ready( data_ready);  



#ifdef WAVE_DUMP
  // Trace files
  sc_trace_file* trace_file = sc_create_vcd_trace_file("trace_behav");

  // Top level signals
  sc_trace(trace_file, clk              , "clk");
  sc_trace(trace_file, rst              , "rst");
 
   sc_trace(trace_file, in_real          , "in_real"); 
   sc_trace(trace_file,  in_imag         , "in_imag");
   sc_trace(trace_file, data_valid       , "data_valid"); 
   sc_trace(trace_file,  data_ack        , "data_ack");

   sc_trace(trace_file, out_real         , "out_real"); 
   sc_trace(trace_file,  out_imag        , "out_imag");
   sc_trace(trace_file, data_req         , "data_req"); 
   sc_trace(trace_file,  data_ready      , "data_ready");
 

#endif  // End WAVE_DUMP

  sc_start( 25, SC_NS );
  rst.write(0);

  sc_start( 25, SC_NS );
  rst.write(1);

  sc_start();

#ifdef WAVE_DUMP
   sc_close_vcd_trace_file(trace_file);
#endif

    return 0;

};
