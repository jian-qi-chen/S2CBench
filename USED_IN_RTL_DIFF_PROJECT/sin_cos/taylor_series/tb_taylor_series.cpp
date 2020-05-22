#include "tb_taylor_series.h"
/* send data */
void test_taylor_series::send(){
    int in_read;
    
    in_file = fopen(INFILENAME, "rt");
    if(!in_file){
        printf("Could not open input file.\n");
        sc_stop();
        exit(-1);
    }
    
    while(fscanf(in_file,"%d", &in_read) != EOF){
        in_angle.write( (D_TYPE)((double)in_read/MUL) );
        wait();
    }
    
    wait();
    
    fclose(in_file);
    sc_stop();
}

/* receive data */
void test_taylor_series::recv(){
//    int sin_read, cos_read;
    D_TYPE sin_read, cos_read;
    
    out_file = fopen(OUTFILENAME, "wt");
    if(!out_file){
        printf("Could not open output file.\n");
        sc_stop();
        exit(-1);
    }
    
    wait();
    while(1){
        sin_read = out_sin.read();
        cos_read = out_cos.read();
        fprintf(out_file, "%d, %d\n", (int)(sin_read*MUL), (int)(cos_read*MUL));
//        fprintf(out_file, "%f, %f\n", sin_read, cos_read);
        wait();
    }
}
