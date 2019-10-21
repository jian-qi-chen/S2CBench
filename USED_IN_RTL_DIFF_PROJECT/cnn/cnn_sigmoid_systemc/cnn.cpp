//========================================================================================
// 
// File Name    : cnn.cpp
// Description  : Convolutional neural network
// Release Date : 10/11/2019
// Author       : Maurice Peemen, Dongrui She 
//                Jianqi Chen, Benjamin Carrion Schafer
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Version    Author                Description
//---------------------------------------------------------------------------------------
// 2013         1.0       M. Peemen, D. She     original pure C implementation          
// 10/11/2019   1.1       Jianqi Chen           convert into synthesizable systemC
//=======================================================================================

#include "cnn.h"

void cnn::cnn_main(void){
  int i;
  static unsigned char in_image[720*1280];//for input image
  //feature map results due to unroling+2 otherwise writes outside array
  static unsigned char net_layer1[6*358*638];
  static unsigned char net_layer2[16*177*317];
  static unsigned char net_layer3[80*173*313];

  static sc_fixed<18,12,SC_RND,SC_SAT> bias1[6]={ 
    #include "bias1.dat"
  };  //memory for network coefficients
  static sc_fixed<12,8,SC_RND,SC_SAT> weight1[6*36]={
    #include "weight1.dat"
  };
  static sc_fixed<18,12,SC_RND,SC_SAT> bias2[16] = {
    #include "bias2.dat"
  };
  static sc_fixed<12,8,SC_RND,SC_SAT> weight2[(6*3+9*4+6)*36] = {
    #include "weight2.dat"
  };
  static sc_fixed<18,12,SC_RND,SC_SAT> bias3[80] = {
    #include "bias3.dat"
  };
  static sc_fixed<12,8,SC_RND,SC_SAT> weight3[25*8*80] = {
    #include "weight3.dat"
  };
  static sc_fixed<18,12,SC_RND,SC_SAT> bias4[8] = {
    #include "bias4.dat"
  };
  static sc_fixed<12,8,SC_RND,SC_SAT> weight4[80*8] = {
    #include "weight4.dat"
  };
  
  unsigned int detectarray[3*10];
  int detections;
  
  wait();
  while(1){  

    // read image
    for(i=0; i<720*1280; i++){
      in_image[i] = indata.read();
      wait();
    }

        
    //perform feed forward operation through the network
    run_convolution_layer1(in_image, net_layer1, bias1, weight1);
    run_convolution_layer2(net_layer1, net_layer2, bias2, weight2);
    run_convolution_layer3(net_layer2, net_layer3, bias3, weight3);
    detections=run_convolution_layer4(net_layer3, bias4, weight4, detectarray);

    // write detection
    detection_out.write( (sc_uint<8>)detections );

    // write detectarray
    for(i=0; i<3*10; i++){
      outdata.write( detectarray[i] );
      wait();
    }
    wait();
  }

}

/************************************************************************************
 * Input   : input image, pointer to output result, coefficients bias and weights
 * Output  : neuron outputs of the feature maps represented as an image
 * Procedure: perform feed forward computation through the feature extraction layers
     *******************************************************************************/
void cnn::run_convolution_layer1(unsigned char in_layer[], unsigned char out_layer[],
                            const sc_fixed<18,12,SC_RND,SC_SAT> bias[], const sc_fixed<12,8,SC_RND,SC_SAT> weight[]) {
  int k,l,m,n,r;
  static sc_fixed<22,16,SC_RND,SC_SAT> y[6*358*638];
  
  //init values of feature maps at bias value
  for(r=0; r<6; r++){
    for(m=0; m<358*638; m++){
      y[r*358*638+m]=bias[r];
    }
  }  

  //loop over output feature maps
  for(r=0; r<6; r++){
    //convolve weight kernel with input image
    for(n=0; n<638; n++){
      for(m=0; m<358; m++){//shift input window over image
        //multiply input window with kernel
        for(l=0; l<6; l++){
          for(k=0; k<6; k++){
            y[r*358*638+m*638+n] += (sc_fixed<22,16,SC_RND,SC_SAT>)(in_layer[(m*2+k)*1280+n*2+l] * weight[r*36+k*6+l]);
          }
        }
      }
    }
  }
  
  //sigmoid activation function
  for(r=0; r<6*358*638; r++){
    out_layer[r]=activation_function(y[r]);
  }
}

/********************************************************************************
 * Input   : input image, pointer to output result, coefficients bias and weights
 * Output  : the neuron outputs computed from the input pattern
 * Procedure: perform feed forward computation through the neural network
 ********************************************************************************/
void cnn::run_convolution_layer2(unsigned char in_layer[], unsigned char out_layer[],
                            const sc_fixed<18,12,SC_RND,SC_SAT> bias[], const sc_fixed<12,8,SC_RND,SC_SAT> weight[]) {
  int k,l,m,n,q,r,qindex;
  static sc_fixed<22,16,SC_RND,SC_SAT> y[16*177*317];
  //feature maps are sparse connected therefore connection scheme is used
  const int qq[60]={0,1,2, 1,2,3, 2,3,4, 3,4,5, 0,4,5, 0,1,5,
                    0,1,2,3, 1,2,3,4, 2,3,4,5, 0,3,4,5, 0,1,4,5, 0,1,2,5,
                    0,1,3,4, 1,2,4,5, 0,2,3,5, 0,1,2,3,4,5};

  //init values of feature map at bias value
  for(r=0; r<16; r++){
    for(q=0; q<177*317; q++){
      y[r*177*317+q]=bias[r];
    }
  }  
            
  //loops over output feature maps with 3 input feature maps
  for(r=0; r<6; r++){
    for(q=0; q<3; q++){//connect with all connected 3 input feature maps
      qindex=qq[r*3+q];//lookup connection address
      //convolve weight kernel with input image
      for(m=0; m<177; m++){//shift input window over image
        for(n=0; n<317; n++){
          //multiply input window with kernel
          for(k=0; k<6; k++){
            for(l=0; l<6; l++){
              y[r*177*317+m*317+n] += (sc_fixed<22,16,SC_RND,SC_SAT>)(in_layer[qindex*358*638+(m*2+k)*638+n*2+l]
                * weight[(r*3+q)*36+k*6+l]);
            }
          }
        }
      }         
    }
  }
    
  for(r=0; r<9; r++){//loop over output feature maps with 4 input maps
    for(q=0; q<4; q++){//connect with all connected 4 input feature maps
      qindex=qq[r*4+q+18];//lookup feature map address
        
      //convolve weight kernel with input image
      for(m=0; m<177; m++){//shift input window over image
        for(n=0; n<317; n++){
          //multiply input window with kernel
          for(k=0; k<6; k++){
            for(l=0; l<6; l++){
              y[(r+6)*177*317+m*317+n]
                += (sc_fixed<22,16,SC_RND,SC_SAT>)(in_layer[qindex*358*638+(m*2+k)*638+n*2+l]
                * weight[(r*4+q+18)*36+k*6+l]);
            }
          }
        }
      }
    }
  }
  
  //compute last feature map connected with all 6 input feature maps
  for(q=0; q<6; q++){//connect with all input feature maps
    qindex=qq[54+q];//lookup the 6 addresses
    
    //convolve weight kernel with input image
    for(m=0; m<177; m++){//shift input window over image
      for(n=0; n<317; n++){
        //multiply input window with kernel
        for(k=0; k<6; k++){
          for(l=0; l<6; l++){
            y[15*177*317+m*317+n]
              += (sc_fixed<22,16,SC_RND,SC_SAT>)(in_layer[qindex*358*638+(m*2+k)*638+n*2+l]
              * weight[(54+q)*36+k*6+l]);
          }
        }
      }
    }
  }

  for(r=0; r<16*177*317; r++){ //sigmoid activation
    out_layer[r]=activation_function(y[r]);
  }
}

/************************************************************************************
 * Input   : input image, pointer to output result, coefficients bias and weights
 * Output  : the neuron outputs computed from the input pattern
 * Procedure: perform feed forward computation through the neural network
 ************************************************************************************/
void cnn::run_convolution_layer3(unsigned char in_layer[], unsigned char out_layer[],
                            const sc_fixed<18,12,SC_RND,SC_SAT> bias[], const sc_fixed<12,8,SC_RND,SC_SAT> weight[]) {
  int k,l,m,n,q,r;
  static sc_fixed<22,16,SC_RND,SC_SAT> y[80*173*313];

  //init values of feature maps at bias value
  for(r=0; r<80; r++){
    for(q=0; q<173*313; q++){
      y[r*173*313+q]=bias[r];
    }  
  }

  for(q=0; q<8; q++){//connect with first 8 feature maps
    for(r=0; r<40; r++){//loops over first 40 output feature maps
      //convolve weight kernel with input image
      for(n=0; n<313; n++){//shift input window over image
        for(m=0; m<173; m++){      
          //multiply input window with kernel
          for(l=0; l<5; l++){//only 5x5 convolution
            for(k=0; k<5; k++){//there is no subsampling in this layer    
              y[r*173*313+m*313+n]
                += (sc_fixed<22,16,SC_RND,SC_SAT>)(in_layer[q*177*317+(m+k)*317+n+l] * weight[(r*8+q)*25+k*5+l]);
            }
          }
        }
      }
    }           
  }
  
  for(q=8; q<16; q++){//connect with last 8 feature maps
    for(r=40; r<80; r++){ //loops over remaining 40 output feature maps 
      //convolve weight kernel with input image
      for(n=0; n<313; n++){//shift input window over image
        for(m=0; m<173; m++){
          //multiply input window with kernel
          for(l=0; l<5; l++){//only 5x5 convolution 
            for(k=0; k<5; k++){     
              y[r*173*313+m*313+n] += (sc_fixed<22,16,SC_RND,SC_SAT>)(in_layer[q*177*317+(m+k)*317+n+l]
                * weight[(r*8+q-8)*25+k*5+l]);
            }
          }
        }       
      }
    }           
  }
  
  for(r=0; r<80*173*313; r++){//sigmoid activation function
    out_layer[r]=activation_function(y[r]);
  }
}

/************************************************************************************
 * Input   : input image, coefficients bias and weights, vote array for detected signs
 * Output  : voting histogram for the signs
 * Procedure: perform feed forward computation through the neural network layer
              threshold with neuron output to detect signs at pixel positions
************************************************************************************/
int cnn::run_convolution_layer4(unsigned char in_layer[], const sc_fixed<18,12,SC_RND,SC_SAT> bias[],
                            const sc_fixed<12,8,SC_RND,SC_SAT> weight[], unsigned int detect[]) {
  int m,n,q,r;
  int detections=0;
  int posx, posy;
  sc_fixed<22,16,SC_RND,SC_SAT> y;
  int set=0;

  sc_fixed<22,16,SC_RND,SC_SAT> max;

  //convolve weight kernel with input image
  for(m=0; m<173; m++){//shift input window over image
    for(n=0; n<313; n++){
      //init values of feature map at bias value
      y = bias[0];
      for(q=0; q<80; q++){
        y += (sc_fixed<22,16,SC_RND,SC_SAT>)(in_layer[q*173*313+m*313+n] * weight[q]);
      }
      // no sigmoid required sigmoid threshold 0.6 => potential should be
      // inverse -ln(0.6^-1 -1)= 0.405 x 256 = 103.799
      //if (y >= 103.799f){ // if sign detected figure out which sign
	  if (y >= 0.0f){ // if sign detected figure out which sign
        max=0;
        for(r=1; r<8; r++){// check other 7 maps for the stronges sign
          y = bias[r];
          for(q=0; q<80; q++){
            y += (sc_fixed<22,16,SC_RND,SC_SAT>)(in_layer[q*173*313+m*313+n] * weight[r*80+q]);
          }
          //if (y>=103.799f && y>max){
		  if (y>=0.0 && y>max){
		    max=y;
            posx=n*4;
			posy=m*4;
			detect[detections*4]=posx;
			detect[detections*4+1]=posy;
			detect[detections*4+2]=r;
			detect[detections*4+3]=0.39 * activation_function(y);
			set=1;
          }
        }
        if (set==1){//this means that a sign is found
          detections=detections+1;
	      set=0;
        }
      }
    }           
  }
  return detections;
}

// sigmoid function
sc_fixed<22,16,SC_RND,SC_SAT> cnn::activation_function( sc_fixed<22,16,SC_RND,SC_SAT> sum){
    int i;
    sc_fixed<22,16,SC_RND,SC_SAT> ret_v, sum_n;
    sc_fixed<24,8,SC_RND,SC_SAT> sum_e;
    sum_e=1;
    sum_n = -sum/256;
    // calculate exp with Taylor series
    for (i=20;i>0;--i)
        sum_e = 1+sum_n*sum_e/i;

    ret_v = 256/(1+sum_e);
    return ret_v;
}