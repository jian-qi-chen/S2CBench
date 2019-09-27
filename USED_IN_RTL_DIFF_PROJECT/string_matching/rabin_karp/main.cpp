#include "define.h"
#include "rabin_karp.h"
#include "tb_rabin_karp.h"

int sc_main(int argc, char** argv)
{
    sc_clock clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
    sc_signal<bool> rst;
    sc_signal< sc_uint<8> > pat_in;
    sc_signal< sc_int<16> > index_out;
    
    rabin_karp u_rk("rabin_karp");
    tb_rabin_karp u_test("tb_rabin_karp");
    
    // connection
    u_rk.clk( clk );
    u_rk.rst( rst );
    u_rk.pat_in( pat_in );
    u_rk.index_out( index_out );
    
    u_test.clk( clk );
    u_test.rst( rst );
    u_test.pat_in( pat_in );
    u_test.index_out( index_out );
    
    // simulation
    sc_start(25, SC_NS);
    rst.write(0);
    
    sc_start(25, SC_NS);
    rst.write(1);
    
    sc_start();
    
    return 0;
}