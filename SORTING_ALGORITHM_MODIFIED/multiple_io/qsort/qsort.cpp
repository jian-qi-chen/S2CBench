//---------------------------------------------------------------------------
//
// File Name    : qsort.cpp
// Description  : Quick sort implementation
// Release Date : 01/08/13
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------
// Date       Version   Author               Description
//---------------------------------------------------------------------------
//            1.0       Darel Rex Finley   Initial version. This public-domain C implementation by
// 01/08/13   1.1       PolyU DARC Lab   Conversion into Synthesizable SystemC
//---------------------------------------------------------------------------

#include "qsort.h"


void quicksort::run(){
	
    sc_uint<8>  data[SIZE]/* Cyber array=EXPAND, array_index=const*/;
    int x;

    wait();

    while(1){
        /* Cyber unroll_times=all */
        for(x = 0 ; x < SIZE ; x++){
            data[x] = indata[x].read();
        }


        // Main quick sort routine 
        sort(data);

        /* Cyber unroll_times=all */
        for(x = 0 ;x < SIZE ;x++){
            odata[x].write(data[x]);
        }


        wait() ;
    }

}

//--------------------------
// Main sorting function
//-------------------------
void quicksort::sort(sc_uint<8> *arr){


    // Variables declaration
    int i=0, L, R;
    sc_uint<8>  piv, beg[SIZE]/* Cyber array=EXPAND, array_index=const*/, end[SIZE]/* Cyber array=EXPAND, array_index=const*/;

    beg[0]=0;
    end[0]=SIZE;

    while (i>=0) {

        L=beg[i]; R=end[i]-1;
        if (L<R) {
            piv=arr[L];

            while (L<R) {
                while (arr[R]>=piv && L<R)
                    R--;
                if (L<R)
                    arr[L++]=arr[R];

                while (arr[L]<=piv && L<R)
                    L++;
                if(L<R) 
                    arr[R--]=arr[L];
            }

            arr[L]=piv;
            beg[i+1]=L+1;
            end[i+1]=end[i];
            end[i++]=L;

            if (end[i]-beg[i]>end[i-1]-beg[i-1]){
                swap(&end[i], &beg[i]);
            }
        }

        else{
            i--;
        }
    } // end while

}


void quicksort::swap(sc_uint<8> *end, sc_uint<8> *beg){

    int swap;

    swap=*beg;
    *beg=*(beg-1);
    *(beg-1)=swap;

    swap=*end;
    *end=*(end-1); 
    *(end-1)=swap; 

}
