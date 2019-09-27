#include "define.h"
#include "matinv_lu.h"
#include "tb_matinv_lu.h"

int sc_main(int argc, char** argv)
{
    sc_clock clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
    sc_signal<bool> rst;
    sc_signal< DATA_TYPE > indata;
    sc_signal< DATA_TYPE > outdata;
    
    matinv_lu u_matinv("matinv_lu");
    tb_matinv_lu u_test("tb_matinv_lu");
    
    // connection
    u_matinv.clk( clk );
    u_matinv.rst( rst );
    u_matinv.indata( indata );
    u_matinv.outdata( outdata );
    
    u_test.clk( clk );
    u_test.rst( rst );
    u_test.indata( indata );
    u_test.outdata( outdata );
    
    // simulation
    sc_start( 25, SC_NS );
    rst.write(0);
    
    sc_start( 25, SC_NS );
    rst.write(1);
    
    sc_start();
    
    return 0;
    
};