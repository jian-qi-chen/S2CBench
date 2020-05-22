#include "bin_search.h"

void bin_search::bin_search_main(void)
{
    sc_ufixed<32,16,SC_RND,SC_SAT> x, x2, in_read, min, max; 
    wait();
    
    while(1){
        in_read = in_num.read();
        
        if(in_read<1){
            min = in_read;
            max = 1;
        }
        else{
            min = 1;
            max = in_read;
        }
        
        x = in_read;
        x2 = x*x;
        while(x2-in_read>ERROR || in_read-x2>ERROR){
            if(x2>in_read){
                max = x;
                x = (max+min)/2;
            }
            else{
                min = x;
                x = (max+min)/2;
            }
            x2 = x*x;
        }
        
        out_num.write(x);
        
        wait();
    }
    
}
