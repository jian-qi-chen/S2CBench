#include "tb_str_match.h"

void tb_str_match::send(){
    int i;
    unsigned char pattern_read[MAX_PAT_LEN]; 
    
    in_data_file = fopen(INFILE,"rt");
    
    if(!in_data_file){
        printf("Could not open input file\n");
        sc_stop();
        exit(-1);
    }
    
    fscanf(in_data_file, "%[^\n\r]", &pattern_read);
    wait();
    for(i=0; i<MAX_PAT_LEN; i++){
        pat_in.write( (sc_uint<8>)pattern_read[i] );
        wait();
        if(pattern_read[i]==0)
            break;
    }
    
    fclose(in_data_file);
    
    while(1){
        wait();
    }
}

void tb_str_match::recv(){
    int index_read=0;
    
    out_data_file = fopen(OUTFILE, "wt");
    fprintf(out_data_file,"Pattern found at index:\n");
    
    if(!out_data_file){
        printf("Could not open output file\n");
        sc_stop();
        exit(-1);
    }
    
    wait();
    wait();
    bool stop_flg = false;
    
    while(1){
        index_read = (int)index_out.read();
        if(index_read != -1 ){
            stop_flg = true;
            printf("Pattern found at index %d\n", index_read);
            fprintf(out_data_file, "%d\n", index_read);
        }
        if( (stop_flg == true)  && (index_read == -1) )
            break;
        wait();
    }
    
    fclose(out_data_file);
    
    sc_stop();
    wait();
}
    
    