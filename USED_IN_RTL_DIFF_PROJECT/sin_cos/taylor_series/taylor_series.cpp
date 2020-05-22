#include "taylor_series.h"

void taylor_series::taylor_series_main(void)
{
    int k;
    D_TYPE z;
    sc_fixed<16,3,SC_RND,SC_SAT> s, c; //sin, cos, 
    sc_fixed<20,7,SC_RND,SC_SAT> n;//numerator
    sc_uint<16> d;//denominator
    bool s_sign, c_sign;
    wait();
    
    while(1){
        z = in_angle.read();
        
        s = 0;
        c = 1;
        n = 1;
        d = 1;
        s_sign = 1;
        c_sign = 0;

        for (k=1; k<=8; k++)
        {
            n *= z;
            d *= k;
            if (k%2==1){
                if(s_sign==1)
                    s += (n/d);
                else
                    s -= (n/d);
                    
                s_sign = !s_sign;
            }
            else{
                if(c_sign==1)
                    c += (n/d);
                else
                    c -= (n/d);
                    
                c_sign = !c_sign;
            }
                
        }  
//        printf("%f: %f, %f\n",(double)z,(double)s,(double)c);//debug
        out_cos.write( c);
        out_sin.write( s);

        wait();
    }
}
