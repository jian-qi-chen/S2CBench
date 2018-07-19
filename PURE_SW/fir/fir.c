#include "stdio.h"
#include "stdlib.h"
#include <time.h>

#define FILTER_TAPS 9
#define TEST_VECTOR_NUM 1000000

#define INFILTERFILENAME         "fir_in_data.txt"   // random data
#define INCOEFFFILENAME          "fir_coeff.txt"

#define OUTFILENAME_GOLDEN       "fir_output_golden.txt"
#define OUTFILENAME              "fir_output.txt"
#define DIFFFILENAME             "firdiff.txt"

int in_data;
int coeff[FILTER_TAPS];
int filter_output;
int data_buffer[FILTER_TAPS];

FILE * in_filter_file, *in_coeff_file, *out_filter_golden_file, *out_filter_file_read;
FILE  *out_filter_file, *diff_file;

// Filter function
int filter( int  *ary, int  *coeff)
{
    int sop=0;
    int i ;

    // Sum of product (SOP) generation 
    for(i=0;i<FILTER_TAPS;i++){
        sop += coeff[i] * ary[i];
    }

    // Sign adjustment and rounding to sc_unit <8>)
    if ( sop < 0 ){
        sop = 0 ;
    }

    return sop;
}

//  Main thread  
void fir_main ( void ) {

    // Variables declaration
    int filter_output_function;
    int in_data_read;
    int coeff_read[FILTER_TAPS];
    int i,j;
		
    // read inputs
    in_data_read = in_data;
    for(i=0;i<FILTER_TAPS;i++){
        coeff_read[i] = coeff[i];
    }

    // shift
    for(j=FILTER_TAPS-1;j>0;j--){
        data_buffer[j] = data_buffer[j-1];
    }
    data_buffer[0] = in_data_read;

    // Filter function
    filter_output_function = filter(data_buffer, coeff_read);
    
    // write outputs
    filter_output = filter_output_function ;

}

int main(){
    int i=0;
	unsigned int in_filter_read;
    unsigned int filter_out_write=0;
	float coeff_read;

	//Reset routine
	in_filter_file = fopen(INFILTERFILENAME, "rt");
	in_coeff_file = fopen(INCOEFFFILENAME, "rt");

	if(!in_filter_file){
		printf( "Could not open %s\n",INFILTERFILENAME);
		exit (-1);
	}

	if(!in_coeff_file){
        printf( "Could not open %s\n",INCOEFFFILENAME);
		exit (-1);
	}

	in_data=0;
	for(i=0; i < FILTER_TAPS; i ++){
		if(fscanf(in_coeff_file, "%f", &coeff_read) == EOF)
			break;
		coeff[i] = coeff_read;
	}
    
    // out_filter_file.open (OUTFILENAME);
	out_filter_file = fopen (OUTFILENAME, "wt");

	if(!out_filter_file){
        printf( "Could not open %s\n",OUTFILENAME);
		exit (-1);
	}
    
    // read inputs
    int in_data_arr[TEST_VECTOR_NUM];
    int filter_output_arr[TEST_VECTOR_NUM];
    
    i = 0;
    while(fscanf(in_filter_file,"%u", &in_filter_read) != EOF){
        in_data_arr[i] = in_filter_read;
        i++;
    }
    
    clock_t begin = clock();
    // execution
    for(i=0; i<TEST_VECTOR_NUM; i++){
        in_data = in_data_arr[i];
        fir_main();
        filter_output_arr[i] = filter_output;
    }
    clock_t end = clock();
    
    double time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
    
    // write outputs
    for(i=0; i<TEST_VECTOR_NUM; i++){
        filter_out_write = filter_output_arr[i];
        fprintf(out_filter_file,"%d\n",filter_out_write);
    }
        
 /*   
    while(fscanf(in_filter_file,"%u", &in_filter_read) != EOF){
        in_data = in_filter_read;
        filter_out_write = filter_output;
        fir_main();
        fprintf(out_filter_file,"%d\n",filter_out_write);
    }
*/
    fclose(in_coeff_file);
    fclose(in_filter_file);
    fclose(out_filter_file);
    
    printf("Execution Time = %f s\n",time_spent);
    
}

	

