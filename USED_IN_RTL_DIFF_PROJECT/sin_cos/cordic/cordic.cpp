// orginal c code: http://www.dcs.gla.ac.uk/~jhw/cordic/

#include "cordic.h"

void cordic::cordic_main(void)
{
    sc_int<16> cordic_ctab [] = {0x3243, 0x1DAC, 0x0FAD, 0x07F5, 0x03FE, 0x01FF, 0x00FF, 0x007F, 0x003F, 0x001F, 0x000F, 0x0007, 0x0003, };
    sc_int<16> k, d, tx, ty, tz;
    sc_int<16> x,y,z,zz;
    
    wait();
    
    while(1){
        z = in_angle.read();
        x = cordic_1K;
        y = 0;

        for (k=0; k<CORDIC_NTAB; ++k)
        {
            d = z>=0 ? 0:-1;
            //d = z>>15;
            
            tx = x - (((y>>k) ^ d) - d);
            ty = y + (((x>>k) ^ d) - d);
            tz = z - ((cordic_ctab[k] ^ d) - d);
            x = tx; y = ty; z = tz;
        }  
        
        out_cos.write(x);
        out_sin.write(y);

        wait();
    }
}
