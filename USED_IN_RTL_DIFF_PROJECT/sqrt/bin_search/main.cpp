#include "define.h"
#include "bin_search.h"
#include "tb_bin_search.h"

int sc_main(int argc, char** argv)
{
    sc_clock               clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
    sc_signal<bool>        rst;
    
    sc_signal<sc_ufixed<32,16,SC_RND,SC_SAT> > in_num;
    sc_signal<sc_ufixed<32,16,SC_RND,SC_SAT> > out_num;
    
    bin_search u_bin_search("bin_search");
    test_bin_search test("test_bin_search");
    
    //connect to bin_search
    u_bin_search.clk( clk );
    u_bin_search.rst( rst );
    u_bin_search.in_num( in_num );
    u_bin_search.out_num( out_num );
    
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
