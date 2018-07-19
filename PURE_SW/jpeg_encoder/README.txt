This jpeg_encoder.c does 4 things:
1. contains a synthesizable JPEG encoder written in BDL
2. contains a software version JPEG encoder
3. generate input .tlv files for hardware simulation
4. create a JPEG file from output .tlv files

How to play with this code:
1. Extract all files into your current directory. There is simple Makefile included. Just enter command "make io_hw". It will generate jpeg_encoder.exe and make a new directory ./iofiles/ under your current directory.
2. Run jpeg_encoder.exe. The software version encoder will encode the "lena512.bmp" and generate "lena512_out.jpg". And all input .tlv files will be generated in ./iofiles.
3. Synthesize the jpeg_encoder_BDL.c in CWB. No analysis options is needed. Import the input .tlv files for cycle accurate simulation. And get output .tlv files as the results of the simulation.
4. copy the output .tlv files into ./iofiles/ and run jpeg_encoder.exe again in your current directory. "lena_512_HW_out.jpg" will be generate from the output .tlv files.

When synthesizing the jpeg encoder with CWB, "dct_coefficient_int.dat"(which is different from "dct_coefficient.dat" used in software version) along with other 3 .dat files need to be included. 