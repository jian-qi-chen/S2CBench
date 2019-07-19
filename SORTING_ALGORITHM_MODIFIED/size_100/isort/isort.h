#ifndef QSORT_H
#define QSORT_H

#include "define.h"


SC_MODULE( insertionsort )
{
    sc_in<bool> clk;
    sc_in<bool> rst;

   sc_in<sc_uint<8> > indata;
   sc_out<sc_uint<8> > odata;


   /* R */
   void run() ;

   /* S */
   void swap(sc_uint<8> *, sc_uint<8> *);
   void sort(sc_uint<8> *);

	
   SC_CTOR( insertionsort)
     {
       SC_CTHREAD(run, clk.pos()) ;
       reset_signal_is(rst, false) ;
     }

   ~insertionsort(){}

};

#endif // QOSRT_H

