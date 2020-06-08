#include "tb_matinv_adj.h"

void tb_matinv_adj::send(){
    int i,j;
    float in_read;

    in_data_file = fopen(INFILE, "rt");
    
    if(!in_data_file){
        printf("Could not open input file\n");
        sc_stop();
        exit(-1);
    }
    
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            fscanf(in_data_file, "%f", &in_read);
            indata.write((DATA_TYPE)in_read);
            wait();
        }
    }
    
    fclose(in_data_file);
    
    while(1){
        wait();
    }
}

void tb_matinv_adj::recv(){
    int i,j;
    float out_write;
    
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            wait();
        }
    }
    
    out_data_file = fopen(OUTFILE, "wt");
    
    if(!out_data_file){
        printf("Could not open output file\n");
        sc_stop();
        exit(-1);
    }
    
    wait();
    wait();
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            out_write = (float)outdata.read();
            fprintf(out_data_file,"%f\n",out_write);
            wait();
        }
    }
    printf("outputs wrote to %s\n",OUTFILE);
    sc_stop();
    wait();
}

 