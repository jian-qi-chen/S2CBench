#include "babylonian.h"

void babylonian::babylonian_main(void)
{
    sc_ufixed<32,16,SC_RND,SC_SAT> x, y, in_read;
    wait();
    
    while(1){
        in_read = in_num.read();
        x = in_read;
        y = 1;
        
        while(x-y>ERROR || y-x>ERROR){
            x = (x+y)/2;
            y = in_read/x;
        }
        
        out_num.write(x);
        wait();
    }
    
}
