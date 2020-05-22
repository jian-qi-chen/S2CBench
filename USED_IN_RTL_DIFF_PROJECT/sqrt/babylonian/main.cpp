#include "define.h"
#include "babylonian.h"
#include "tb_babylonian.h"

int sc_main(int argc, char** argv)
{
    sc_clock               clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
    sc_signal<bool>        rst;
    
    sc_signal<sc_ufixed<32,16,SC_RND,SC_SAT> > in_num;
    sc_signal<sc_ufixed<32,16,SC_RND,SC_SAT> > out_num;
    
    babylonian u_babylonian("babylonian");
    test_babylonian test("test_babylonian");
    
    //connect to babylonian
    u_babylonian.clk( clk );
    u_babylonian.rst( rst );
    u_babylonian.in_num( in_num );
    u_babylonian.out_num( out_num );
    
    //connect to testbench
    test.clk( clk );
    test.rst( rst );
    test.in_num( in_num );
    test.out_num( out_num );
    
    //Start simulation
    sc_start( 25, SC_NS );
    rst.write(0);

    sc_start( 25, SC_NS );
    rst.write(1);

    sc_start();
    
    return 0;
    
}
