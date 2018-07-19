#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define TAPS 8

#define IN_DATA   "in_data.txt"
#define IN_FACTOR_DATA  "in_factor_data.txt"

#define GOLDEN_OUTPUT "odata_golden.txt"
#define GOLDEN_OUTPUT_ENABLE "odata_enable_golden.txt"
#define DIFF_OUT "diff.txt"

#define OUT_FILE   "odata.txt"
#define OUT_FILE_ENABLE "odata_enable.txt"

#define INPUT_VECTOR_NUM 10000 // when EX_TIME is defined input vector number*100

FILE *fptr_data;
FILE *fptr_en;
FILE *in_file_data, *in_file_factor;

double indata;
bool odata_en;
double odata[4];
double buffer[TAPS];

const double coeff1[TAPS] ={-0.002109243232750,
                           0.017755702728024,
                           -0.077066788574370,
                           0.287601741388967,
                           0.862310282276647,
                           -0.107706831291022,
                           0.021638102269905,
                           -0.002423612146178};

const double coeff2[TAPS] = {-0.003143633003860,
                           0.027185874557551,
                           -0.125223235153495,
                           0.601180543048866,
                           0.601180543048866,
                           -0.125223235153495,
                           0.027185874557551,
                           -0.003143633003860};

const double coeff3[TAPS] = {-0.002423612146178,
                           0.021638102269905,
                           -0.107706831291022,
                           0.862310282276647,
                           0.287601741388967,
                           -0.077066788574370,
                           0.017755702728024,
                           -0.002109243232750};

const double coeff4[TAPS-1] = {-0.000000000617302,
                           0.000000003212201,
                           -0.000000007388632,
                           1.000000009584823,
                           -0.000000007388632,
                           0.000000003212201,
                           -0.000000000617302
                           };


void interp_run ( void ) {
	unsigned int n;
    
    double indata_read;
    double odata_write;

    double SoP1;
    double SoP2;
    double SoP3;
    double SoP4;
				

      // Read inputs
    indata_read = indata;
      
    odata_en = 0;

    // Read inputs by shifting previous data
    for ( n = TAPS-1; n>0; n--)
        buffer[n] = buffer[n-1];
    buffer[0] = indata_read;

      // FIR 1 : Sum of Products of 1st filter
    SoP1 = 0;
    for (n = 0; n < TAPS; n++ ) 
        SoP1 = SoP1 + buffer[n] * coeff1[n];
      
      // FIR 2 : Sum of Products of 2nd filter
    SoP2 = 0;
    for (n = 0; n < TAPS; n++ ) 
        SoP2 =SoP2 + buffer[n] * coeff2[n];

      // FIR 3 : Sum of Products of 3rd filter
    SoP3 = 0;
    for ( n = 0; n < TAPS; n++ ) 
        SoP3 = SoP3 + buffer[n] * coeff3[n];

      // FIR 4 : Sum of Products of 4th filter
    SoP4 = 0;
    for (n = 0; n < TAPS-1; n++ ) 
        SoP4 = SoP4 + buffer[n] * coeff4[n];

     // Write results back
    odata_en = 1;
    odata[0] = SoP1;
    odata[1] = SoP2;
    odata[2] = SoP3;
    odata[3] = SoP4;


}

int main(){
    float indata_var;
    bool odata_en_rcv;
    double  odata_rcv;
	int i,j;


	in_file_data = fopen(IN_DATA, "r");
    fptr_data = fopen(OUT_FILE , "wt");

	if(!in_file_data){
        printf("\n%s data file could not be opened\n", IN_DATA);
        exit(1);
	}

    // read inputs
    double indata_arr[INPUT_VECTOR_NUM];
    double odata_arr[INPUT_VECTOR_NUM*4];
    
    i = 0;
    while(fscanf(in_file_data,"%f\n",&indata_var) !=EOF ){
        indata_arr[i] = indata_var;
        i++;
    }
    
    clock_t begin = clock();
#ifdef EX_TIME
int q;
for(q=0;q<100;q++){
#endif
    // execution
    for(i=0;i<INPUT_VECTOR_NUM;i++){
        indata = indata_arr[i];
        interp_run();
        odata_en_rcv = odata_en;
        for(j=0;j<4;j++){
            odata_arr[i*4+j] = odata[j];
        }
    }
#ifdef EX_TIME
}
#endif
    clock_t end = clock();
    double time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
   
    // write results
    j=0;
    for(i=0;i<INPUT_VECTOR_NUM*4;i++){
        j++;
        odata_rcv = odata_arr[i];
        fprintf(fptr_data,"%f\n",odata_rcv);
        if(j==4){
            j = 0;
            fprintf(fptr_data,"\n");
        }
    }

    fclose(in_file_data);
    fclose(fptr_data);
    
    printf("Execution Time = %f s\n", time_spent);
}



