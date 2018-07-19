#define DCT_OUT_WIDTH 20   //DCT output bitwidth
#define QUAN_OUT_WIDTH 9  //quantization output bitwidth
#define RLE_BUFF_LEN 128   //RLE output buffer length

#ifdef C
#include <time.h>
#include <stdbool.h>
#include "define.h"
FILE* ifptr, *ofptr;
/* inputs */
unsigned char input_col[8];

/* outputs */
bool output_col[64];
int last;

/* global variables */
unsigned char line_buffer[8][8];
int dct_output[8][8];
int quantization_output[8][8];
int previous_dc = 0;
int zz[64];
int rl[RLE_BUFF_LEN];
bool encode_output[512];
int last_bit = 0;
bool check_byte[8];
int check_index;
int jpeg_len;
int jpeg_len_b;
unsigned char *jpegFinalImage;
bool *jpegFinalImage_bits;
unsigned char biColourPalette[1024];
BITMAPINFOHEADER bitmapInfoHeader;
BITMAPFILEHEADER bitmapFileHeader;
int i = 0;

#else

/* inputs and outputs */
in ter(0:8) input_col[8];
out var output_col[64];
out var(0:9) last;

/* global variables */
var(0:8) line_buffer[8][8];
signed var(0:DCT_OUT_WIDTH) dct_output[8][8];
signed var(0:QUAN_OUT_WIDTH) quantization_output[8][8];
signed var(0:QUAN_OUT_WIDTH) previous_dc = 0;
signed var(0:QUAN_OUT_WIDTH) zz[64];
signed var(0:QUAN_OUT_WIDTH) rl[RLE_BUFF_LEN];
var encode_output[512];
var(0:9) last_bit = 0;
var check_byte[8];
var(0:3) check_index;
var(0:4) i = 0;

#endif

/* function declarations */
void jpeg_dct(void);
void jpeg_quantization(void);
void jpeg_zigzag(void);
int jpeg_rle(void);
int jpeg_getCat(int a);
#ifdef C
void jpeg_DCcode(int rl0);
unsigned char  *load_bitmapfile(const char *image);
void image_write(void);
unsigned char bitToByte(bool* bitarray);
void createJPEG(void);
#else
void jpeg_DCcode(signed var(0:QUAN_OUT_WIDTH) rl0);
#endif
void jpeg_ACcode(int length);
void jpeg_addzero(void);

/* main function */
#ifdef C
void jpeg_encoder()
#else
process jpeg_encoder()
#endif
{
  int j,k,m,n;
  unsigned int X, Y;
  int rl_length;

    /* shift output buffer(encode_output) by 64 bits */
    if(last_bit<=64)
      last_bit = 0;
    else{
      for(k=64;k<last_bit;k++)
        encode_output[k-64] = encode_output[k];
      last_bit = last_bit - 64;
    }

    /* shift 8x8 line buffer by one column */
    for(Y=0;Y<7;Y++){
      for(X=0;X<8;X++){
        line_buffer[X][Y]=line_buffer[X][Y+1];
      }
    }

    /* read new data into the line buffer */
    for(X=0;X<8;X++)
      line_buffer[X][7] = input_col[X];

  /* wait 8 cycles to get new 8x8 block */
    i++;
    if(i==8){
      i = 0;

      /* perform DCT of the 8x8 block */
      jpeg_dct();

      /* quantize the DCT coefficients */
      jpeg_quantization();
    
      /* rearrange the quantized block in zigzag order  */
      jpeg_zigzag();
     
      /* run-length encoding */
      rl_length = jpeg_rle();

      /* get DC code */
      jpeg_DCcode(rl[0]-previous_dc);
      previous_dc = rl[0];

      /* get AC code */
      jpeg_ACcode(rl_length);
  
      /* put 0x00 after every 0xFF byte */
      jpeg_addzero();
    }

      /* write output */
    last = last_bit;
#ifndef C
    output(last);
#endif
    for(j=0;j<64;j++){
      output_col[j] = encode_output[j];
#ifndef C
      output(output_col[j]);
#endif  
  }
#ifndef C
  return 0;
#endif
}

void jpeg_dct()
{
  int u,v,x,y;
#ifdef C
  double a;
  double dct_co[8][8] = {
    #include "dct_coefficient.dat"
  };
#else
  int a;
  int dct_co[8][8] = {
    #include "dct_coefficient_int.dat"
  };
#endif

  for(u=0;u<8;u++)
    for(v=0;v<8;v++)
    {
      a = 0;
      for(x=0;x<8;x++)
        for(y=0;y<8;y++)
#ifdef C
          a+= (double)line_buffer[x][y]*dct_co[x][u]*dct_co[y][v];
      if(u==0){
        if(v==0) dct_output[u][v] = (0.25*0.5*a-1024);
        else dct_output[u][v] = (0.25*0.707*a);
      }
      else {
        if(v==0) dct_output[u][v] = (0.25*0.707*a);
        else dct_output[u][v] = (0.25*a);
      }
#else
          a+= (line_buffer[x][y])*(dct_co[x][u]*dct_co[y][v]);

      if(u==0){
        if(v==0) dct_output[u][v] = a/8 - 1024*16*16;
        else dct_output[u][v] = 10*a/57;
      }
      else {
        if(v==0) dct_output[u][v] = 10*a/57;
        else dct_output[u][v] = a/4;
      }
      dct_output[u][v] >>= 8;
#endif
    }
}

void jpeg_quantization()
{
  /*quantization matrix*/
  int Q[8][8] =  {
    #include "quantization_matrix.dat"
  };
  int i,j;

  for(i=0;i<8;i++)
    for(j=0;j<8;j++)
     quantization_output[i][j] = (dct_output[i][j]/Q[i][j]);
}

void jpeg_zigzag()
{
  int i=0,j=0,k=0,d=0;
  
  /*zigzag for the top left half of the input block*/
  while(k<36)
  {
    zz[k++] = quantization_output[i][j];
    if((i==0)&&(j%2==0))
    {
      j++;
      d = 1;
    }
    else if((j==0)&&(i%2==1))
    {
      i++;
      d = 0;
    }
    else if(d==0)
    {
      i--;
      j++;
    }
    else
    {
      i++;
      j--;
    }
  }

  i = 7;
  j = 1;
  d = 0;

  /*zigzag for the bottom right half of the input block*/
  while(k<64)
  {
    zz[k++] = quantization_output[i][j];
    if((i==7)&&(j%2==0))
    {
      j++;
      d = 0;
    }
    else if((j==7)&&(i%2==1))
    {
      i++;
      d = 1;
    }
    else if(d==0)
    {
      i--;
      j++;
    }
    else
    {
      i++;
      j--;
    }
  }
}

int jpeg_rle()
{
  int len = 1; /*length of RL code*/
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
    if(i==64)  /*end of block*/
    {
      rl[len++] = 0;
      rl[len++] = 0;
    }
    else
    {
      rl[len++] = k; /*number of zeros before a non-zero number*/
      rl[len++] = zz[i++];  /*non-zero number*/
    }
  }

  while((rl[len-4]==15)&&(rl[len-3]==0))
  {
    rl[len-4]=0;
    len-=2;
  }

  return len;
}
  
int jpeg_getCat(int a)  /*get index of 1st part of DC code*/
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

#ifdef C
void jpeg_DCcode(int rl0)
#else
void jpeg_DCcode(signed var(0:QUAN_OUT_WIDTH) rl0)
#endif
{
  int powertable[12] = {1,2,4,8,16,32,64,128,256,512,1024,2048};
  int code_len[12] = {3,4,5,5,7,8,10,12,14,16,18,20};
#ifdef C
  bool code[12][9] =
#else
  var code[12][9] =
#endif
   {{0,1,0},{0,1,1},{1,0,0},{0,0},{1,0,1}, {1,1,0},{1,1,1,0},{1,1,1,1,0},{1,1,1,1,1,0},{1,1,1,1,1,1,0},{1,1,1,1,1,1,1,0},{1,1,1,1,1,1,1,1,0}};

  int cat = jpeg_getCat(rl0);
  int dclen = code_len[cat];  /* length of the DC code in # of bits */
#ifdef C
  bool dc_code[20];
#else
  var dc_code[20];
#endif
  /* get first part of DC code in string */
  int q, j, k;
  int c = rl0;
  for(q=0;q<(dclen-cat);q++)
    dc_code[q] = code[cat][q];
  
  /* get second part of DC code in string */
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

  /* write to output register */
  for(k=0;k<dclen;k++){
    encode_output[last_bit+k] = dc_code[k];
  } 

  last_bit += dclen; /* add length of DC code */
}

void jpeg_ACcode(int length)
{
  int powertable[12] = {1,2,4,8,16,32,64,128,256,512,1024,2048};
  int codeLen[16][11] = {
    #include "AClen.dat"
  };
#ifdef C
  bool code[176][16] = {
#else
  var code[176][16] = {
#endif 
    #include "ACcode.dat"
  };
  
  int i,j;

  for(i=1;i<length;i+=2)
  {   
    int num_zero = rl[i];
    int cat =  jpeg_getCat(rl[i+1]);
    int aclen = codeLen[num_zero][cat];  /* length of AC code in # of bits */
     
#ifdef C
    bool ac_code[26];
#else
    var ac_code[26];
#endif
    /* get first part of AC code */
    int q, j, k;
    int c = rl[i+1];
    for(q=0;q<(aclen-cat);q++)
      ac_code[q] = code[num_zero*11+cat][q];


    /* get second part of AC code */
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

    /* write to output register */
    for(k=0;k<aclen;k++)
      encode_output[last_bit+k] = ac_code[k];

    last_bit += aclen; /* add length of AC code */ 

  }

} 

void jpeg_addzero(){
  int m,k,i;
  int n = 0;/* # of 0xFF */

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


/* C testbench */
#ifdef C
int main(){

#ifdef IODATA
  int s;
  FILE *in[8];
  FILE *out[64];
  FILE *last_f;
  in[0] = fopen("iofiles/input_col_a00.tlv","wb");
  in[1] = fopen("iofiles/input_col_a01.tlv","wb");
  in[2] = fopen("iofiles/input_col_a02.tlv","wb");
  in[3] = fopen("iofiles/input_col_a03.tlv","wb");
  in[4] = fopen("iofiles/input_col_a04.tlv","wb");
  in[5] = fopen("iofiles/input_col_a05.tlv","wb");
  in[6] = fopen("iofiles/input_col_a06.tlv","wb");
  in[7] = fopen("iofiles/input_col_a07.tlv","wb");
/*  last_f = fopen("iofiles/last.tlv","wb");

  char out_file_name[64][29]; //outputfile
  for(s=0;s<64;s++){
    out_file_name[s][0] = '.';
    out_file_name[s][1] = '/';
    out_file_name[s][2] = 'i';
    out_file_name[s][3] = 'o';
    out_file_name[s][4] = 'f';
    out_file_name[s][5] = 'i';
    out_file_name[s][6] = 'l';
    out_file_name[s][7] = 'e';
    out_file_name[s][8] = 's';
    out_file_name[s][9] = '/';
    out_file_name[s][10] = 'o';
    out_file_name[s][11] = 'u';
    out_file_name[s][12] = 't';
    out_file_name[s][13] = 'p';
    out_file_name[s][14] = 'u';
    out_file_name[s][15] = 't';
    out_file_name[s][16] = '_';
    out_file_name[s][17] = 'c';
    out_file_name[s][18] = 'o';
    out_file_name[s][19] = 'l';
    out_file_name[s][20] = '_';
    out_file_name[s][21] = 'a';

    if(s<10){
      out_file_name[s][22] = '0';
      sprintf(&out_file_name[s][23],"%d",s);
    }
    else
      sprintf(&out_file_name[s][22],"%d",s);

    out_file_name[s][24] = '.';
    out_file_name[s][25] = 't';
    out_file_name[s][26] = 'l';
    out_file_name[s][27] = 'v';
    out_file_name[s][28] = '\0';
    
    out[s] = fopen(out_file_name[s],"wb");
    if(!out[s]){
      printf("Could not open output file");
      break;
    }
  }
*/

  

  if(!in[0]||!in[1]||!in[2]||!in[3]||!in[4]||!in[5]||!in[6]||!in[7])
    printf("Could not open I/O files\n");
  char newline = '\n';
#endif

  // Input initialization
  static int i,j,k,m;
  unsigned char input_col_write[8];

  unsigned char *bitmapData_r = load_bitmapfile("lena512.bmp");
  // put the data upside down
  unsigned char *bitmapData = (unsigned char*) malloc(512*512);
  for(i=0;i<512;i++)
    for(j=0;j<512;j++)
      bitmapData[512*512-(i+1)*512+j] = bitmapData_r[i*512+j];

  //Output initialization
  bool output_read[64];
  ofptr=fopen("lena512_out.jpg","wb");

  jpegFinalImage = (unsigned char *)malloc(512*512);
  jpegFinalImage_bits = (bool *)calloc(8*512*512,sizeof(bool));
  if (!jpegFinalImage||!jpegFinalImage_bits)
    {
      free(jpegFinalImage);
      free(jpegFinalImage_bits);
      printf("Memory not allocated");  
    }

  jpeg_len = 0;
  jpeg_len_b = 0;

  clock_t begin = clock();

    // Send Image data to jpeg encoder
    for(i=0;i<(ROWS/8);i++){
      for(j=0;j<COLS;j++){
	      for(k=0;k<8;k++){
	        // Send 8 pixels at a time
	        input_col_write[k] = bitmapData[(i*COLS*8)+j+(k*COLS)];
	        input_col[k] = input_col_write[k];  
	      }
        jpeg_encoder(); //jpeg encode

#ifdef IODATA
        char str[8][4]; //for inputs
        int len[8];
        for(k=0;k<8;k++){
          sprintf(str[k],"%d",(int)input_col[k]);
          if(input_col[k]<10)
            len[k] = 1;
          else if(input_col[k]<100)
            len[k] = 2;
          else
            len[k] = 3;
        }
              
/*        char str_o[64]; //for outputs
        for(s=0;s<64;s++)
          sprintf(&str_o[s],"%d",(int)output_col[s]); 

        char str_l[9]; //output-last
        int len_l = 1;
        int tmp_l = last/10;
        while(tmp_l!=0){
          len_l++;
          tmp_l /= 10;
        }
        sprintf(str_l,"%d",last); */


        for(k=0;k<8;k++){
          fwrite(str[k],1,len[k],in[k]);
          fwrite(&newline,1,1,in[k]);
        }
/*        for(s=0;s<64;s++){ //outputfile
          fwrite(&str_o[s],1,1,out[s]);
          fwrite(&newline,1,1,out[s]);
        }
        fwrite(str_l,1,len_l,last_f);
        fwrite(&newline,1,1,last_f);*/
#endif

        if(last>=64){
          for(k=0;k<64;k++){
            output_read[k] = output_col[k];
            jpegFinalImage_bits[jpeg_len_b+k] = output_read[k];
          }
          jpeg_len_b += 64;
        }

        else if(last!=0){
          for(k=0;k<last;k++){
            output_read[k] = output_col[k];
            jpegFinalImage_bits[jpeg_len_b+k] = output_read[k];
          }
          jpeg_len_b += last;
        }

      }
    }
    
    clock_t end = clock();
    double time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
    
#ifdef IODATA
    for(k=0;k<8;k++)
      fclose(in[k]);
/*    fclose(last_f);
    for(s=0;s<64;s++) //outputfile
      fclose(out[s]);*/
#endif
    
    for(m=0;m<7;m++){
      jpeg_encoder();
      if(last>=64){
        for(k=0;k<64;k++){
          output_read[k] = output_col[k];
          jpegFinalImage_bits[jpeg_len_b+k] = output_read[k];
        }
        jpeg_len_b += 64;
      }

      else if(last!=0){
        for(k=0;k<last;k++){
          output_read[k] = output_col[k];
          jpegFinalImage_bits[jpeg_len_b+k] = output_read[k];
        }
        jpeg_len_b += last;
      }
    }


    image_write();        // Generate output file
#ifdef CREATEJPEG
    createJPEG();
#endif

    printf("Execution Time = %f s\n",time_spent);

  }



 unsigned char  *load_bitmapfile(const char *image)
{
 
  unsigned char *bitmapImage;
  int l;

 // Open jpg file
  ifptr = fopen(image,"rb");
  if(!ifptr){
    printf( "Cannot open the file ");
  }


 fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,ifptr);

#ifdef PRINTD
   printf("%c Bitmap identifies\n",bitmapFileHeader.bfType);
    printf("%d Bitmap identifies the size of file\n",bitmapFileHeader.bfSize);
#endif

 if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(ifptr);
	
    }

 // Read the bitmap info header
 fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,ifptr);

 // Read colour palette
 fread(&biColourPalette,1,bitmapInfoHeader.biClrUsed*4,ifptr);


    //move file point to the begging of bitmap data
    fseek(ifptr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapFileHeader.bfSize);

    //verify memory allocation
    if (!bitmapImage)
      {
	printf( "Cannot allocate memory for image");
	free(bitmapImage);
	return NULL;
	}

    //read in the bitmap image data
    fread(bitmapImage,1, bitmapInfoHeader.biSizeImage,ifptr);

    if (bitmapImage == NULL)
      {
	      printf( "Data could not be read" );
	return NULL;
	}



    fclose(ifptr);
    return bitmapImage;
}

unsigned char bitToByte(bool* bitarray)
{
  unsigned char ret = 0;
  int i;
  for(i=0; i<8; ++i){
    if(bitarray[i])
      ret |= 1 << (7-i);
  }
  return ret;
}


void image_write(void){

  // Variables declaration
  int i,j,k,p;
  unsigned char  *tk;
  unsigned char sof[13],soi[2], eoi[2], dqt[69], dhtDC[33], dhtAC[183], sos[10];

  //write SOI (start of image)
  soi[0] = 255;
  soi[1] = 216;
  fwrite(soi,1,2,ofptr);

  //write DQT (quantization table)
  dqt[0] = 255;
  dqt[1] = 219;  //marker
  dqt[2] = 0;   //length
  dqt[3] = 67;  //length
  dqt[4] = 0;   //8-bit Y component
  unsigned char Q[64] = {
    #include "quantization_matrix.dat"
  };

  for(j=0;j<64;j++)
    dqt[j+5] = Q[j];

  fwrite(dqt,1,69,ofptr);

  //write SOF (start of frame)
  sof[0] = 255;
  sof[1] = 192; //baseline sequential DCT mode
  sof[2] = 0;
  sof[3] = 11;//length
  sof[4] = 8; //# of bits for color value
  sof[5] = 2; //width /256
  sof[6] = 0; //width %256
  sof[7] = 2; //height /256
  sof[8] = 0; //height %256
  sof[9] = 1; //# of components (grayscale=1)
  sof[10] = 0; // component identifier
  sof[11] = 17; // 0b00010001 (H,V)=(1,1)
  sof[12] = 0; //quantization table destination selector
  fwrite(sof,1,13,ofptr);
  
  //write DHT (DC Huffman table)
  dhtDC[0] = 255;
  dhtDC[1] = 196;
  dhtDC[2] = 0;
  dhtDC[3] = 31;  //length
  dhtDC[4] = 0;  //DC, Y component
  unsigned char BITS_DC[16] = {0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0};
  for(k=0;k<16;k++)
    dhtDC[k+5] = BITS_DC[k];

  unsigned char HUFFVAL_DC[12] =  {3,0,1,2,4,5,6,7,8,9,10,11};
  for(k=0;k<12;k++)
    dhtDC[k+21] = HUFFVAL_DC[k];

  fwrite(dhtDC,1,33,ofptr);

  //write DHT (AC Huffman table)
  dhtAC[0] = 255;
  dhtAC[1] = 196;
  dhtAC[2] = 0;
  dhtAC[3] = 181; //length
  dhtAC[4] = 16; // 0b00010000
  unsigned char BITS_AC[16] = {0,2,1,3,2,4,4,3,5,5,5,2,0,0,1,125};
  for(k=0;k<16;k++)
    dhtAC[k+5] = BITS_AC[k];

  unsigned char HUFFVAL_AC[162] = 
{1, 2, 3, 0, 4, 17, 5, 33, 6, 18, 49, 65, 7, 19, 81, 97, 34, 113, 129, 20, 50, 145, 161, 177, 8, 35, 66, 82, 193, 21, 51, 98, 114, 209, 225, 240, 130, 9, 10, 22, 23, 24, 25, 26, 36, 37, 38, 39, 40, 41, 42, 52, 53, 54, 55, 56, 57, 58, 67, 68, 69, 70, 71, 72, 73, 74, 83, 84, 85, 86, 87, 88, 89, 90, 99, 100, 101, 102, 103, 104, 105, 106, 115, 116, 117, 118, 119, 120, 121, 122, 131, 132, 133, 134, 135, 136, 137, 138, 146, 147, 148, 149, 150, 151, 152, 153, 154, 162, 163, 164, 165, 166, 167, 168, 169, 170, 178, 179, 180, 181, 182, 183, 184, 185, 186, 194, 195, 196, 197, 198, 199, 200, 201, 202, 210, 211, 212, 213, 214, 215, 216, 217, 218, 226, 227, 228, 229, 230, 231, 232, 233, 234, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250};

  for(k=0;k<162;k++)
    dhtAC[k+21] = HUFFVAL_AC[k];

  fwrite(dhtAC,1,183,ofptr);

  //write SOS (start of scan)
  sos[0] = 255;
  sos[1] = 218;
  sos[2] = 0;
  sos[3] = 8;
  sos[4] = 1; //# of components
  sos[5] = 0; //Y component identifier
  sos[6] = 0; //DC,AC selector
  sos[7] = 0;
  sos[8] = 63;
  sos[9] = 0;
  fwrite(sos,1,10,ofptr);

  //write data
  jpeg_len = (int)(jpeg_len_b/8)+1;
  //put 1's in remainder of the last byte
  for(p=0;p<7;p++)
    jpegFinalImage_bits[jpeg_len_b+p] = 1;
  for(p=0;p<jpeg_len;p++){
    jpegFinalImage[p] = bitToByte(&jpegFinalImage_bits[p*8]);
  }

  tk = (unsigned char  *)calloc(jpeg_len,sizeof(unsigned char));

  if(ofptr!=NULL){
    for(i=0;i<jpeg_len;i++){
      tk[i] = jpegFinalImage[i];
    }
   
    fwrite(tk,1,jpeg_len,ofptr);
  }

  else{
    printf("\nFile not opened \n");
  }


  //write EOI (end of image)
  eoi[0] = 255;
  eoi[1] = 217;
  fwrite(eoi,1,2,ofptr);

 fclose(ofptr);
 free(tk);

}

#ifdef CREATEJPEG
#define OUT_NUMBER 32768
void createJPEG(){
	FILE *outputptr[64];
	FILE *lastptr;
	free(jpegFinalImage);
	free(jpegFinalImage_bits);
	jpegFinalImage = (unsigned char *)calloc(OUT_NUMBER*8,sizeof(unsigned char));
	jpegFinalImage_bits = (bool *)calloc(OUT_NUMBER*64,sizeof(bool));
	jpeg_len_b = 0;
	jpeg_len = 0;
	char output_name[27] = {'i','o','f','i','l','e','s','/','o','u',
        't','p','u','t','_','c','o','l','_',
	'a','0','0','.','t','l','v','\0'};
	int s,n,m;

	for(s=0;s<64;s++){
		output_name[20] = s/10 + '0';
		output_name[21] = s%10 + '0';
		outputptr[s] = fopen(output_name,"rb");
		if(!outputptr[s]){
			printf("Cannot open output file\n");
			printf("Please put output files in ./iofiles/ to generate image\n");
			return;
		}
	}
	lastptr = fopen("iofiles/last.tlv","rb");
	if(!lastptr) printf("Cannot open last.tlv\n");

	bool output[64];
	int last;
	unsigned char tmp;
	for(n=0;n<OUT_NUMBER;n++){
		last = 0;
		while(1){
			fread(&tmp,1,1,lastptr);
			if(tmp != '\n') last = last*10 + (tmp-'0');
			else break;
		}

		for(s=0;s<64;s++){
			fread(&tmp,1,1,outputptr[s]);
			output[s] = tmp - '0';
			fread(&tmp,1,1,outputptr[s]);
		}

		if(last<64){
			for(s=0;s<last;s++)
				jpegFinalImage_bits[jpeg_len_b+s] = output[s];
			jpeg_len_b += last;
		}

		else{
			for(s=0;s<64;s++)
				jpegFinalImage_bits[jpeg_len_b+s] = output[s];
			jpeg_len_b += 64;
		}

	}
	
	for(s=0;s<64;s++)
		fclose(outputptr[s]);
	fclose(lastptr);

	ofptr = fopen("lena_512_HW_out.jpg","wb");
	image_write();

}
#endif
#endif
  








