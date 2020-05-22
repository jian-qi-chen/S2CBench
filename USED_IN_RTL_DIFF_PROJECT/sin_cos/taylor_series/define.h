#ifndef DEFINE_H_
#define DEFINE_H_

#ifndef SC_INCLUDE_FX
#define SC_INCLUDE_FX
#endif

#include "systemc.h"
#include "stdio.h"

#define MUL 16384

#define INFILENAME "taylor_series_input.txt"
#define OUTFILENAME "taylor_series_output.txt"

#define D_TYPE sc_fixed<16,3,SC_RND,SC_SAT>

#endif
