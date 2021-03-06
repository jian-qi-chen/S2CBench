//========================================================================================
// 
// File Name    : tb_fft.cpp
// Description  : Testbench for FFT
// Release Date : 16/07/2013
// Author       : UTD DARClab
//                Benjamin Carrion Schafer
//
// Revision History
//---------------------------------------------------------------------------------------
// Date      Version    Author            Description
//----------------------------------------------------------------------------------------
//16/07/2013    1.0     PolyU DARC lab    FFT  original testbench (send,receive data and compare results)
//
//=======================================================================================
#include "tb_fft.h"


//--------------------------
// Send data thread
//-------------------------
void test_fft::send(){

  // Variables declaration
  int i=0;
  float  in_read_real, in_read_imag;

  //Reset routine
  in_file = fopen(INFILENAME, "rt");


  if(!in_file){
    cout << "Could not open " << INFILENAME << "\n";
    sc_stop();
    exit(EXIT_FAILURE);
  }


  while(true){

    data_valid.write(false);  // start sending data to FFT module
    while(fscanf(in_file,"%f", &in_read_real) != EOF){
      if(fscanf(in_file,"%f", &in_read_imag) == EOF)
	break;

         

	in_real.write(in_read_real);
	in_imag.write(in_read_imag);
    while(data_req.read() == false)
	   wait();

	//	cout << "Input vector:" << in_read_real << "+i" << in_read_imag << endl;

	wait();
    }
  
    data_valid.write(true);  

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
void test_fft::recv(){

  // Variables declaration
  float out_write_real, out_write_imag;

  out_file = fopen (OUTFILENAME, "wt");

  if(!out_file){
    cout << "Could not open " << OUTFILENAME << "\n";
    sc_stop();
    exit (-1);
  }

  wait();

  while(true){

     while(data_ready.read() == false)
       wait();

    data_ack.write(true);

    out_write_real = out_real.read();
    out_write_imag = out_imag.read();

    fprintf(out_file,"%0.2f ",out_write_real);
    fprintf(out_file,"%0.2f\n",out_write_imag);

    //  cout << "Output vector:" <<out_write_real << "+i" << out_write_imag << endl;

    wait();

    data_ack.write(false);

    wait();

  }
}


//--------------------------
// Compare results function
 //--------------------------
void test_fft::compare_results(){

  float outfft_real, outfft_imag, out_golden_real, out_golden_imag;
  int  line=1, errors=0;

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
  out_file_golden = fopen (OUTFILENAME_GOLDEN, "rt");
  if(!out_file_golden){
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

    while(fscanf(out_file_golden, "%f", &out_golden_real) != EOF){
      if(fscanf(out_file_golden, "%f", &out_golden_imag) == EOF)
	break;


      if(fscanf(out_file,"%f", &outfft_real) == EOF)
	break;
      if(fscanf(out_file,"%f", &outfft_imag) ==  EOF)
	break;

      cout << endl <<"Cycle["<< line << "]: " << out_golden_real << "+i" << out_golden_imag << " -- "<< outfft_real <<"+i" << outfft_imag;

      const float error_tolerance = 1e-5;
      float error_real = fabsf((outfft_real - out_golden_real) / out_golden_real);
      float error_imag = fabsf((outfft_imag - out_golden_imag) / out_golden_imag);
      if(error_real > error_tolerance || error_imag > error_tolerance){
	cout << "\nOutput missmatch [line:" << line << "] Golden:" << out_golden_real << "+i" << out_golden_imag << " -- Output:" << outfft_real << "+i" << outfft_imag;

	fprintf(diff_file,"\nOutput missmatch[line:%d] Golden: %0.16f+i%0.f -- Output: %0.16f+i%0.f",line, out_golden_real, out_golden_imag,  outfft_real, outfft_imag);
	
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
    fclose(out_file_golden);



}

