#include "tb_isort.h"


//--------------------------
// Send data thread
//-------------------------
void test_isort::send(){

  // Variables declaration
  int i=0;
  unsigned  in_read;

  //Reset routine
  in_file = fopen(INFILENAME, "rt");


  if(!in_file){
    cout << "Could not open " << INFILENAME << "\n";
    sc_stop();
    exit(EXIT_FAILURE);
  }

  

    while(true){
        i = 0;
        while(fscanf(in_file,"%d", &in_read) != EOF){
            indata[i++].write(in_read);
            if (i==SIZE){
                i = 0;
                wait();
            }                
        }

        wait();

        fclose(in_file);
        cout << endl << "Starting comparing results " << endl;

        compare_results();
        sc_stop();

        wait();

    }//while_loop
}



//--------------------------
// Receive data thread
//--------------------------
void test_isort::recv(){

  // Variables declaration
  unsigned int out_write=0;
  int i;

  out_file = fopen (OUTFILENAME, "wt");

  if(!out_file){
    cout << "Could not open " << OUTFILENAME << "\n";
    sc_stop();
  }


  wait();

  while(true){

    for(i=0;i<SIZE; i++){
        out_write = odata[i].read();
        fprintf(out_file,"%d\n",out_write);
    }

    //	cout << "\nReading "  << out_write;

    wait();
  }
}


//--------------------------
// Compare results function
//--------------------------
void test_isort::compare_results(){

  int outsort, out_golden, line=1, errors=0;

  // Close file where outputs are stored
  fclose(out_file);

  out_file = fopen (OUTFILENAME, "rt");

  if(!out_file){
    cout << "Could not open " << OUTFILENAME << "\n";
    sc_stop();
    exit(EXIT_FAILURE);
  }

    //
    //Load the golden pattern
    //
      out_golden_file = fopen (OUTFILENAME_GOLDEN, "rt");
     if(!out_golden_file){
      cout << "Could not open " << OUTFILENAME_GOLDEN << "\n";
      sc_stop();
      exit(EXIT_FAILURE);
     }

    //
    //Dump the comparison result
    //
    diff_file = fopen (DIFFFILENAME, "w");
    if(!diff_file){
	 cout << "Could not open " << DIFFFILENAME<< "\n";
	 sc_stop();
       }

    while(fscanf(out_golden_file, "%d", &out_golden) != EOF){
      fscanf(out_file,"%d", &outsort);
     

      cout << endl <<"Cycle["<< line << "]: " << out_golden << "-- "<< outsort;

      if(outsort != out_golden){
	cout << "\nOutput missmatch [line:" << line << "] Golden:" << out_golden << " -- Output:" << outsort;

	fprintf(diff_file,"\nOutput missmatch[line:%d] Golden: %d -- Output: %d",line, out_golden, outsort);
	
	errors++;
      }
      
      line ++;

    }

    if(errors == 0)
      cout << endl << "Finished simulation SUCCESSFULLY" << endl;
    else
      cout << endl << "Finished simulation " << errors << " MISSMATCHES between Golden and Simulation" << endl;


    fclose(out_file);
    fclose(diff_file);
    fclose(out_golden_file);



}

