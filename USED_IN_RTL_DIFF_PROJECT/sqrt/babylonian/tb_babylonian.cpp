#include "tb_babylonian.h"
/* send data */
void test_babylonian::send(){
    float in_read;
    
    in_file = fopen(INFILENAME, "rt");
    if(!in_file){
        printf("Could not open input file.\n");
        sc_stop();
        exit(-1);
    }
    
    while(fscanf(in_file,"%f", &in_read) != EOF){
        in_num.write( (sc_ufixed<32,16,SC_RND,SC_SAT>)in_read );
        wait();
    }
    
    wait();
    
    fclose(in_file);
    sc_stop();
}

/* receive data */
void test_babylonian::recv(){
    sc_ufixed<32,16,SC_RND,SC_SAT> out_read;
    
    out_file = fopen(OUTFILENAME, "wt");
    if(!out_file){
        printf("Could not open output file.\n");
        sc_stop();
        exit(-1);
    }
    
    wait();
    while(1){
        out_read = out_num.read();
        fprintf(out_file, "%f\n", (double)out_read);
        wait();
    }
}
