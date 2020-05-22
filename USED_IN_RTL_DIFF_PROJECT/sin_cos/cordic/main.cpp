#include "define.h"
#include "cordic.h"
#include "tb_cordic.h"

int sc_main(int argc, char** argv)
{
    sc_clock               clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
    sc_signal<bool>        rst;
    
    sc_signal<sc_int<16> > in_angle;
    sc_signal<sc_int<16> > out_sin;
    sc_signal<sc_int<16> > out_cos;
    
    cordic u_cordic("cordic");
    test_cordic test("test_cordic");
    
    //connect to CORDIC
    u_cordic.clk( clk );
    u_cordic.rst( rst );
    u_cordic.in_angle( in_angle );
    u_cordic.out_sin( out_sin );
    u_cordic.out_cos( out_cos );
    
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
