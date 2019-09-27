//---------------------------------------------------------------------------
// 
// File Name    : fft.cpp
// Description  : FFT implementation
// Release Date : 16/07/13
// Author       : 
//
// Revision History
//---------------------------------------------------------------------------
// Date     Version   Author          Description
//---------------------------------------------------------------------------
//            1.0   R.Goswami,Synopsys, Inc.  fft.cpp - This is the implementation file for the synchronous process "fft"
//16/07/03    1.1   PolyU DARClab	Change format to S2Cbench style with testbench
//13/04/2017  1.2   UTD DARClab         Converted floats to fixed point data types to make synthesizable                  
//---------------------------------------------------------------------------

/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#include "fft.h"


void fft::entry()
{ 

    // Variables declaration
    //float sample[16][2];  // Note: Float is not synthesizable 
    sc_fixed<32,16, SC_RND, SC_SAT> sample[16][2];
    unsigned int index;

    // Reset state -- should be executable in a single clock cycle
    for (unsigned int i = 0; i < 16; i++) {
        for(unsigned h=0; h <2; h++){
            sample[i][h] = 0;
        }
    }
    index = 0;

    wait();

    // Main computational loop
    while(true)
    { 

        data_req.write(false);
        data_ready.write(false);


        //Reading in the Samples
        for( index = 0; index < 16; index++ )
        {
            data_req.write(true);
            do {
                wait(); 
            } while (data_valid.read() == true);

            sample[index][0] = in_real.read();
            sample[index][1] = in_imag.read();
            printf("index: %d; %f, %f\n", index, (float)sample[index][0], (float)sample[index][1]);

            data_req.write(false);
            wait();
        }
        index = 0;


        //////////////////////////////////////////////////////////////////////////
        ///  Computation - 1D Complex DFT In-Place DIF Computation Algorithm  ////
        //////////////////////////////////////////////////////////////////////////

        //Size of FFT, N = 2**M    
        unsigned int N, M, len ;   
        //float theta;   // Note: float is non-synthesizble as indicated in the SystemC synthesizable subset draft 1.3
        //float  W[7][2], w_real, w_imag, w_rec_real, w_rec_imag, w_temp;
        const sc_fixed<32,4, SC_RND, SC_SAT> theta=0.392699, w_real=0.92388, w_imag=-0.382683; 
        sc_fixed<32,16, SC_RND, SC_SAT>  W[7][2],w_rec_real, w_rec_imag, w_temp;	


        //Initialize
        M = 4; N = 16; 
        len = N/2;
        //     theta = 8*atan(1.0)/N;  // subsitutited by constant in declaration


        //Calculate the W-values recursively
        //  w_real =  cos(theta);    // substituted by constant in declaration
        //  w_imag =  -sin(theta);   // substituted by contant in declaration

        //cout << endl << "theta:" << theta << ", w_real:" << w_real << ", w_imag:" << w_imag;

        w_rec_real = 1;
        w_rec_imag = 0;

        for(index=0; index<len-1; index++)
        {
            w_temp = w_rec_real*w_real - w_rec_imag*w_imag;
            w_rec_imag =  w_rec_real*w_imag + w_rec_imag*w_real;
            w_rec_real = w_temp;
            W[index][0] = w_rec_real;
            W[index][1] = w_rec_imag;
        }


        //     float tmp_real, tmp_imag, tmp_real2, tmp_imag2;
        sc_fixed<32,16, SC_RND, SC_SAT>  tmp_real, tmp_imag, tmp_real2, tmp_imag2;
        unsigned int stage, i, j,index2, windex, incr;

        //Begin Computation 

        len = N;
        incr = 1;

        for(stage=0; stage<M; stage++)
        { 
            len = len/2;

            //First Iteration :  With No Multiplies

            for(i=0; i<N; i=i+2*len)
            {
                index =  i; index2 = index + len; 

                tmp_real = sample[index][0] + sample[index2][0];
                tmp_imag = sample[index][1] + sample[index2][1];

                sample[index2][0] = sample[index][0] - sample[index2][0];
                sample[index2][1] = sample[index][1] - sample[index2][1];

                sample[index][0] = tmp_real;
                sample[index][1] = tmp_imag;
            }


            //Remaining Iterations: Use Stored W
            windex = incr - 1;
            for ( j=1;j < len;j++) // This loop executes N/2 times at first stage, .. once at last stage.
            { 
                for (i=j; i < N; i=i+2*len)
                {
                    index = i;
                    index2 = index + len;

                    tmp_real = sample[index][0] + sample[index2][0]; 
                    tmp_imag = sample[index][1] + sample[index2][1];
                    tmp_real2 = sample[index][0] - sample[index2][0];
                    tmp_imag2 = sample[index][1] - sample[index2][1];

                    sample[index2][0] = tmp_real2*W[windex][0] - tmp_imag2*W[windex][1];
                    sample[index2][1] = tmp_real2*W[windex][1] + tmp_imag2*W[windex][0]; 

                    sample[index][0] = tmp_real;
                    sample[index][1] = tmp_imag;

                }
                windex = windex + incr;
            }
            incr = 2*incr;
        } 

        //////////////////////////////////////////////////////////////////////////

        //Writing out the normalized transform values in bit reversed order
        sc_uint<4> bits_i;
        sc_uint<4> bits_index;
        bits_i = 0;

        //    cout << "Writing the transform values..." << endl;
        for( i=0;i < 16; i++)
        {
            bits_i = i;
            bits_index[3]= bits_i[0];
            bits_index[2]= bits_i[1];
            bits_index[1]= bits_i[2];
            bits_index[0]= bits_i[3];
            index = bits_index;

            out_real.write(sample[index][0]);
            out_imag.write(sample[index][1]);
            data_ready.write(true);


            do { 
                wait();
            } while ( !(data_ack.read() == true) );

            data_ready.write(false);
            wait();
        }
        index = 0; 
        // cout << "Done..." << endl;
    }
}
