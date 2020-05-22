#include "define.h"
#include "taylor_series.h"
#include "tb_taylor_series.h"

int sc_main(int argc, char** argv)
{
    sc_clock               clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
    sc_signal<bool>        rst;
    
    sc_signal<D_TYPE > in_angle;
    sc_signal<D_TYPE > out_sin;
    sc_signal<D_TYPE > out_cos;
    
    taylor_series u_taylor_series("taylor_series");
    test_taylor_series test("test_taylor_series");
    
    //connect to taylor_series
    u_taylor_series.clk( clk );
    u_taylor_series.rst( rst );
    u_taylor_series.in_angle( in_angle );
    u_taylor_series.out_sin( out_sin );
    u_taylor_series.out_cos( out_cos );
    
    //connect to testbench
    test.clk( clk );
    test.rst( rst );
    test.in_angle( in_angle );
    test.out_sin( out_sin );
    test.out_cos( out_cos );
    
    //Start simulation
    sc_start( 25, SC_NS );
    rst.write(0);

    sc_start( 25, SC_NS );
    rst.write(1);

    sc_start();
    
    return 0;
    
}
