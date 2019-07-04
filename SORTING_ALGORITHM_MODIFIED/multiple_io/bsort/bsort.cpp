#include "bsort.h"


void bubblesort::run(){
	
    sc_uint<8>  data[SIZE];
    int x;

    wait();

    while(1){

        for(x = 0 ; x < SIZE ; x++){
            data[x] = indata[x].read();
        }


        // Main quick sort routine 
        sort(data);


        for(x = 0 ;x < SIZE ;x++){
            odata[x].write(data[x]);
        }


        wait() ;
    }


}

//--------------------------
// Main sorting function
//-------------------------
void bubblesort::sort(sc_uint<8> *arr){
    int i;
    bool swapped=true;

    while(swapped){
        swapped = false;
        for(i=0; i<SIZE-1; i++){
            if(arr[i]>arr[i+1]){
                swap(&arr[i], &arr[i+1]);
                swapped=true;
            }
        }
    }

}


void bubblesort::swap(sc_uint<8> *a1, sc_uint<8> *a2){

    sc_uint<8> swap;

    swap=*a1;
    *a1=*a2;
    *a2=swap;

}
