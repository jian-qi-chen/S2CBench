#define SC_INCLUDE_FX
#include "define.h"
#include "jpeg_encoder.h"

void jpeg_encoder::jpeg_main(void)
{
  int i,j,k;
  unsigned int X, Y;
  sc_int<QUAN_OUT_WIDTH> zz[64];  //zigzag output
  sc_int<QUAN_OUT_WIDTH> rl[RLE_BUFF_LEN];  //rle output
  int rl_length;
  sc_int<QUAN_OUT_WIDTH> rl0;

  
  i = 0;
  previous_dc = 0;
  last_bit = 0;
  
  wait();

  while(1)
  {
    //shift output buffer(encode_output) by 64 bits
    if(last_bit<=64)
      last_bit = 0;
    else{
      for(k=64;k<last_bit;k++)
        encode_output[k-64] = encode_output[k];
      last_bit = last_bit - 64;
    }

    //shift 8x8 line buffer by one column
    for(Y=0;Y<7;Y++){
      for(X=0;X<8;X++){
        line_buffer[X][Y]=line_buffer[X][Y+1];
      }
    }

    //read new data into the line buffer
    for(X=0;X<8;X++)
      line_buffer[X][7] = input_col[X].read();

    
    // wait 8 cycles to get new 8x8 block
    i++;
    if(i==8){
      i=0;

      // perform DCT of the 8x8 block
      jpeg_dct(line_buffer);

      // quantize the DCT coefficients
      jpeg_quantization(dct_output);
    
      // rearrange the quantized block in zigzag order
      jpeg_zigzag(quantization_output,zz);
      
      // run-length encoding 
      rl_length = jpeg_rle(zz,rl);

      // get DC code
      rl0 = rl[0]-previous_dc;
      jpeg_DCcode(rl0);
      previous_dc = rl[0];

      // get AC code
      jpeg_ACcode(rl,rl_length);

      // put 0x00 after every 0xFF byte
      jpeg_addzero();


    }

    // write output
    last.write(last_bit);
    for(j=0;j<64;j++)
      output[j].write(encode_output[j]);
  

    wait();
  }
}
 

void jpeg_encoder::jpeg_dct(sc_uint<8> input_block[8][8])
{
  int u,v,x,y;
  sc_fixed<18,15,SC_RND,SC_SAT> a;

  sc_fixed<6,2,SC_RND,SC_SAT> dct_co[8][8] = {
    #include "dct_coefficient.dat"
  };

  for(u=0;u<8;u++)
    for(v=0;v<8;v++)
    {
      a = 0;
      for(x=0;x<8;x++)
        for(y=0;y<8;y++)
          a+= (sc_fixed<18,15,SC_RND,SC_SAT>)input_block[x][y]*dct_co[x][u]*dct_co[y][v];
      if(u==0){
        if(v==0) dct_output[u][v] = (0.25*0.5*a-1024);
        else dct_output[u][v] = (0.25*0.707*a);
      }
      else {
        if(v==0) dct_output[u][v] = (0.25*0.707*a);
        else dct_output[u][v] = (0.25*a);
      }
    }
}

void jpeg_encoder::jpeg_quantization(sc_int<DCT_OUT_WIDTH> dct_block[8][8])
{
  //quantization matrix
  sc_int<8> Q[8][8] =  {
    #include "quantization_matrix.dat"
  };
  int i,j;

  for(i=0;i<8;i++)
    for(j=0;j<8;j++)
     quantization_output[i][j] = (sc_int<QUAN_OUT_WIDTH>)(dct_block[i][j]/Q[i][j]);
}

void jpeg_encoder::jpeg_zigzag(sc_int<QUAN_OUT_WIDTH> input[8][8],sc_int<QUAN_OUT_WIDTH> zz[64])
{
  int i;
  int x_index[64] = {0,0,1,2,1,0,0,1,2,3,4,3,2,1,0,0,1,2,3,4,5,6,5,4,3,2,1,0,0,1,2,3,
                     4,5,6,7,7,6,5,4,3,2,1,2,3,4,5,6,7,7,6,5,4,3,4,5,6,7,7,6,5,6,7,7};
  int y_index[64] = {0,1,0,0,1,2,3,2,1,0,0,1,2,3,4,5,4,3,2,1,0,0,1,2,3,4,5,6,7,6,5,4,
                     3,2,1,0,1,2,3,4,5,6,7,7,6,5,4,3,2,3,4,5,6,7,7,6,5,4,5,6,7,7,6,7};

  for( i=0; i<64; i++ )
	  zz[i] = input[x_index[i]][y_index[i]];

}

int jpeg_encoder::jpeg_rle(sc_int<QUAN_OUT_WIDTH> zz[64],sc_int<QUAN_OUT_WIDTH> rl[RLE_BUFF_LEN])
{
  int len = 1; //length of RL code
  int i = 1;
  int k = 0;
  rl[0] = zz[0];

  while(i<64)
  {
    k=0;
    while((i<64)&&(zz[i]==0)&&(k<15))
    {
      i++;
      k++;
    }
    if(i==64)  //end of block
    {
      rl[len++] = 0;
      rl[len++] = 0;
    }
    else
    {
      rl[len++] = k; //number of zeros before a non-zero number
      rl[len++] = zz[i++];  //non-zero number
    }
  }

  while((rl[len-4]==15)&&(rl[len-3]==0))
  {
    rl[len-4]=0;
    len-=2;
  }

  return len;
}
  
int jpeg_encoder::jpeg_getCat(int a)  //get index of 1st part of DC code
{
  if(a==0)
	return 0;
  else if((a<=1)&&(a>=-1))
	return 1;
  else if((a<=3)&&(a>=-3))
	return 2;
  else if((a<=7)&&(a>=-7))
	return 3;
  else if((a<=15)&&(a>=-15))
	return 4;
  else if((a<=31)&&(a>=-31))
	return 5;
  else if((a<=63)&&(a>=-63))
	return 6;
  else if((a<=127)&&(a>=-127))
	return 7;
  else if((a<=255)&&(a>=-255))
	return 8;
  else if((a<=511)&&(a>=-511))
        return 9;
  else if((a<=1023)&&(a>=-1023))
        return 10;
  else
	return 11;
}

void jpeg_encoder::jpeg_DCcode(sc_int<QUAN_OUT_WIDTH> rl0)
{
  int powertable[12] = {1,2,4,8,16,32,64,128,256,512,1024,2048};
  int code_len[12] = {3,4,5,5,7,8,10,12,14,16,18,20};
  sc_uint<1> code[12][9] = {{0,1,0},{0,1,1},{1,0,0},{0,0},{1,0,1}, {1,1,0},{1,1,1,0},{1,1,1,1,0},{1,1,1,1,1,0},{1,1,1,1,1,1,0},{1,1,1,1,1,1,1,0},{1,1,1,1,1,1,1,1,0}};

  int cat = jpeg_getCat(rl0);
  int dclen = code_len[cat];  //length of the DC code in # of bits
  sc_uint<1> dc_code[20];
  //get first part of DC code in string
  int q;
  for(q=0;q<(dclen-cat);q++)
    dc_code[q] = code[cat][q];
  
  int j;
  int c = rl0;
  //get second part of DC code in string
  if(rl0<0)
    c+= powertable[cat]-1;
  for(j=dclen-1;j>dclen-cat-1;j--)
  {
    if(c%2==1)
      dc_code[j] = 1;
    else
      dc_code[j] = 0;
    c/=2;
  }

  //write to output register
  int k;
  for(k=0;k<dclen;k++){
    encode_output[last_bit+k] = dc_code[k];
  } 

  last_bit += dclen; //add length of DC code   
}

void jpeg_encoder::jpeg_ACcode(sc_int<QUAN_OUT_WIDTH> rl[RLE_BUFF_LEN],int length)
{
  int powertable[12] = {1,2,4,8,16,32,64,128,256,512,1024,2048};
  int codeLen[16][11] = {
    #include "AClen.dat"
  };
  sc_uint<1> code[176][16] = { 
    #include "ACcode.dat"
  };
  
  int i,j;

  for(i=1;i<length;i+=2)
  {   
    int num_zero = rl[i];
    int cat =  jpeg_getCat(rl[i+1]);
    int aclen = codeLen[num_zero][cat];  //length of AC code in # of bits
     
    sc_uint<1> ac_code [26];
    // get first part of AC code
    int q;
    for(q=0;q<(aclen-cat);q++)
      ac_code[q] = code[num_zero*11+cat][q];


    // get second part of AC code
    int c = rl[i+1];
    if(rl[i+1]<0)
      c += powertable[cat]-1;
    for(j=aclen-1;j>aclen-cat-1;j--)
    {
      if(c%2==1)
        ac_code[j] = 1;
      else
        ac_code[j] = 0;
      c/=2;
    }

    //write to output register
    int k;
    for(k=0;k<aclen;k++)
      encode_output[last_bit+k] = ac_code[k];

    last_bit += aclen; //add length of AC code 

  }

} 

void jpeg_encoder::jpeg_addzero(){
  int m,k,i;
  int n = 0;//# of 0xFF

  for(k=0;k<last_bit;k++){
    check_byte[(check_index+k)%8] = encode_output[k];

    if((check_index+k)%8==7){
      m = check_byte[0]&check_byte[1]&check_byte[2]&check_byte[3]&check_byte[4]&check_byte[5]&check_byte[6]&check_byte[7];
      
      if(m==1){
        for(i=last_bit-1;i>k;i--)
          encode_output[i+8] = encode_output[i];
        for(i=0;i<8;i++)
          encode_output[k+1+i] = 0;
        k += 8;
        last_bit +=8;
        n++;
      }
    }
  }
  check_index = (check_index+last_bit)%8;
}




 
  
