#include "tb_cnn.h"
#include <time.h>

double elapsed;
unsigned char in_image[720*1280];

void tb_cnn::send(void){
  int i;
  char imagename[32]; 
  
  strcpy(imagename, "test000046.pgm");
  read_image_pgm(in_image, imagename, 1280, 720);
  
  wait();
  for( i=0; i<720*1280; i++){
    indata.write( (sc_uint<8>)in_image[i] );
    wait();
  }

  while(true){ 
    wait();
  }
 
}

void tb_cnn::recv(void){
  int i;
  const int max_speed[8]={0, 30, 50, 60, 70, 80, 90, 100};
  time_t end,start=time(NULL);
  unsigned int detectarray[3*10];
  int detections;
  
  wait();
  for(i=0; i<720*1280; i++){ // wait for sending data
    wait();
  }
  
  wait();
  
  for(i=0; i<3*10; i++){
    detectarray[i] = (unsigned int) outdata.read();
    wait();
  }
  detections = (int)detection_out.read();
  
  printf("\nnumber of detections = %d\n",detections);
  
  for(i=0; i<detections; i++){
    printf("detection nr %d = %d km/h, box pos= x %d, y %d, confidence = %d\n",i,max_speed[detectarray[i*4+2]], detectarray[i*4],detectarray[i*4+1],detectarray[i*4+3]);
  }
  
  
  end = time(NULL);
  elapsed = difftime(end, start);
  printf("\n\nRunning time %10.4f\n", elapsed);
  
  annotate_img(in_image, detectarray, detections);
  write_image_pgm(in_image, "output.pgm", 1280, 720); 
  
  sc_stop();
  
}




void tb_cnn::read_image_pgm(unsigned char image[], char filename[], int imageWidth, int imageHeight)
{   /************************************************************************************
     * Function: void read_image_pgm(unsigned char image[], char filename[], int imageWidth, int imageHeight)
     * Input   : uchar array pointer for output result, char array with filename, int with with, int with height
     * Output  : uchar image array
     * Procedure: if image dimensions and layout pgm correct imare is read from file to image array
     ************************************************************************************/
  int grayMax;
  int PGM_HEADER_LINES=3;
  FILE* input;

  int headerLines = 1;
  int scannedLines= 0;
  long int counter =0;

  //read header strings
  char *lineBuffer = (char *) malloc(LINE_BUFFER_SIZE+1);
  char *split;
  char *format = (char *) malloc(LINE_BUFFER_SIZE+1);
  char P5[]="P5";
  char comments[LINE_BUFFER_SIZE+1];

  //open the input PGM file
  input=fopen(filename, "rb");

  //read the input PGM file header
  while(scannedLines < headerLines){
    fgets(lineBuffer, LINE_BUFFER_SIZE, input);
    //if not comments
    if(lineBuffer[0] != '#'){
      scannedLines += 1;
      //read the format
      if(scannedLines==1){
        split=strtok(lineBuffer, " \n");
        strcpy(format,split);
        if(strcmp(format,P5) == 0){
          //printf("FORMAT: %s\n",format);
          headerLines=PGM_HEADER_LINES;
        }
        else
        {
          printf("Only PGM P5 format is support. \n");
        }
      }
      //read width and height
      if (scannedLines==2)
      {
        split=strtok(lineBuffer, " \n");
        if(imageWidth == atoi(split)){ //check if width matches description
          //printf("WIDTH: %d, ", imageWidth);
        }
        else{
          printf("input frame has wrong width should be WIDTH: %d, ", imageWidth);
          exit(4);
        }
        split = strtok (NULL, " \n");
        if(imageHeight == atoi(split)){ //check if heigth matches description
          //printf("HEIGHT: %d\n", imageHeight);
        }
        else{
          printf("input frame has wrong height should be HEIGHT: %d, ", imageHeight);
          exit(4);
        }
      }
      // read maximum gray value
      if (scannedLines==3)
      {
        split=strtok(lineBuffer, " \n");
        grayMax = atoi(split);
        //printf("GRAYMAX: %d\n", grayMax);
      }
    }
    else
    {
      strcpy(comments,lineBuffer);
      //printf("comments: %s", comments);
    }
  }

  counter = fread(image, sizeof(unsigned char), imageWidth * imageHeight, input);
  //printf("pixels read: %d\n",counter);
        
  //close the input pgm file and free line buffer
  fclose(input);
  free(lineBuffer);
  free(format);
}

void tb_cnn::write_image_pgm(unsigned char image[], const char filename[], int imageWidth, int imageHeight){
    FILE * output;
    output = fopen(filename, "wb");
    fprintf(output, "P5\n");
    fprintf(output, "%d %d\n255\n",imageWidth, imageHeight);
    fwrite(image, sizeof(unsigned char), imageWidth * imageHeight, output);
    fclose(output);
}

void tb_cnn::annotate_img(unsigned char img[], unsigned int detectarray[], int detections)
{
  int i,x,y,posx,posy; 
  
  for(i=0; i<detections; i++){
    posx=detectarray[i*4];
	posy=detectarray[i*4+1];
    for(x=0; x<32; x++){
	  img[posy*1280+posx+x]=255;
	  img[(posy+31)*1280+posx+x]=255;
	}
    for(y=1; y<31; y++){
      img[(posy+y)*1280+posx]=255;
	  img[(posy+y)*1280+posx+31]=255;
    }	
  }
}