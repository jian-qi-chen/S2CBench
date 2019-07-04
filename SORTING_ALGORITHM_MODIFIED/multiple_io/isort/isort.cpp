#include "isort.h"


void insertionsort::run(){
	
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
void insertionsort::sort(sc_uint<8> *arr){
    int i = 1, j;

    while(i<SIZE){
        j = i;
        while( j>0 && arr[j-1] > arr[j] ){
            swap(&arr[j], &arr[j-1]);
            j = j-1;
        }
        i = i+1;
    }

}


void insertionsort::swap(sc_uint<8> *a1, sc_uint<8> *a2){

    sc_uint<8> swap;

    swap=*a1;
    *a1=*a2;
    *a2=swap;

}
