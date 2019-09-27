#include "rabin_karp.h"

// txt[] contains "The University of Texas at Dallas is a rising research powerhouse with eight schools and more than 140 academic degrees including top-ranked programs in business, engineering, science, audiology and arts and technology. The campus community is enriched by a commitment to the arts, service learning opportunities and an athletics program with new esports teams. UTD students, faculty, staff and alumni continue to rise to new challenges, proving that Comets shine bright."
const unsigned char txt[TXT_LEN] = {84, 104, 101, 32, 85, 110, 105, 118, 101, 114, 115, 105, 116, 121, 32, 111, 102, 32, 84, 101, 120, 97, 115, 32, 97, 116, 32, 68, 97, 108, 108, 97, 115, 32, 105, 115, 32, 97, 32, 114, 105, 115, 105, 110, 103, 32, 114, 101, 115, 101, 97, 114, 99, 104, 32, 112, 111, 119, 101, 114, 104, 111, 117, 115, 101, 32, 119, 105, 116, 104, 32, 101, 105, 103, 104, 116, 32, 115, 99, 104, 111, 111, 108, 115, 32, 97, 110, 100, 32, 109, 111, 114, 101, 32, 116, 104, 97, 110, 32, 49, 52, 48, 32, 97, 99, 97, 100, 101, 109, 105, 99, 32, 100, 101, 103, 114, 101, 101, 115, 32, 105, 110, 99, 108, 117, 100, 105, 110, 103, 32, 116, 111, 112, 45, 114, 97, 110, 107, 101, 100, 32, 112, 114, 111, 103, 114, 97, 109, 115, 32, 105, 110, 32, 98, 117, 115, 105, 110, 101, 115, 115, 44, 32, 101, 110, 103, 105, 110, 101, 101, 114, 105, 110, 103, 44, 32, 115, 99, 105, 101, 110, 99, 101, 44, 32, 97, 117, 100, 105, 111, 108, 111, 103, 121, 32, 97, 110, 100, 32, 97, 114, 116, 115, 32, 97, 110, 100, 32, 116, 101, 99, 104, 110, 111, 108, 111, 103, 121, 46, 32, 84, 104, 101, 32, 99, 97, 109, 112, 117, 115, 32, 99, 111, 109, 109, 117, 110, 105, 116, 121, 32, 105, 115, 32, 101, 110, 114, 105, 99, 104, 101, 100, 32, 98, 121, 32, 97, 32, 99, 111, 109, 109, 105, 116, 109, 101, 110, 116, 32, 116, 111, 32, 116, 104, 101, 32, 97, 114, 116, 115, 44, 32, 115, 101, 114, 118, 105, 99, 101, 32, 108, 101, 97, 114, 110, 105, 110, 103, 32, 111, 112, 112, 111, 114, 116, 117, 110, 105, 116, 105, 101, 115, 32, 97, 110, 100, 32, 97, 110, 32, 97, 116, 104, 108, 101, 116, 105, 99, 115, 32, 112, 114, 111, 103, 114, 97, 109, 32, 119, 105, 116, 104, 32, 110, 101, 119, 32, 101, 115, 112, 111, 114, 116, 115, 32, 116, 101, 97, 109, 115, 46, 32, 85, 84, 68, 32, 115, 116, 117, 100, 101, 110, 116, 115, 44, 32, 102, 97, 99, 117, 108, 116, 121, 44, 32, 115, 116, 97, 102, 102, 32, 97, 110, 100, 32, 97, 108, 117, 109, 110, 105, 32, 99, 111, 110, 116, 105, 110, 117, 101, 32, 116, 111, 32, 114, 105, 115, 101, 32, 116, 111, 32, 110, 101, 119, 32, 99, 104, 97, 108, 108, 101, 110, 103, 101, 115, 44, 32, 112, 114, 111, 118, 105, 110, 103, 32, 116, 104, 97, 116, 32, 67, 111, 109, 101, 116, 115, 32, 115, 104, 105, 110, 101, 32, 98, 114, 105, 103, 104, 116, 46 }/* Cyber array = RAM */;


void rabin_karp::search(unsigned char pat[], sc_uint<8> M, sc_int<16> index_list[]) //M is length of pattern
{ 
	int i, j; 
	sc_int<8> p = 0; // hash value for pattern 
	sc_int<8> t = 0; // hash value for txt 
	sc_int<8> h = 1; 

	// The value of h would be "pow(ABSZ, M-1)%Q" 
	for (i = 0; i < M-1; i++) 
		h = (h*ABSZ)%Q; 

	// Calculate the hash value of pattern and first 
	// window of text 
	for (i = 0; i < M; i++) 
	{ 
		p = (ABSZ*p + pat[i])%Q; 
		t = (ABSZ*t + txt[i])%Q; 
	} 

    int counter = 0;
	// Slide the pattern over text one by one 
	for (i = 0; i <= TXT_LEN - M; i++) 
	{ 

		// Check the hash values of current window of text 
		// and pattern. If the hash values match then only 
		// check for characters on by one 
		if ( p == t ) 
		{ 
			/* Check for characters one by one */
			for (j = 0; j < M; j++) 
			{ 
				if (txt[i+j] != pat[j]) 
					break; 
			} 

			// if p == t and pat[0...M-1] = txt[i, i+1, ...i+M-1] 
			if (j == M) {
                index_list[counter] = (sc_int<16>)i;
                counter++;
            }
		} 

		// Calculate hash value for next window of text: Remove 
		// leading digit, add trailing digit 
		if ( i < TXT_LEN-M ) 
		{ 
			t = (ABSZ*(t - txt[i]*h) + txt[i+M])%Q; 

			// We might get negative value of t, converting it 
			// to positive 
			if (t < 0) 
                t = (t + Q); 
		} 
	} 
    
    if(counter!=0)
        index_list[counter] = -1;
    else{
        index_list[0] = -2;
        index_list[1] = -1;
    }
}

void rabin_karp::rk_main(void){
    sc_uint<8> i, m;
    unsigned char in_char, pat[MAX_PAT_LEN];
    sc_int<16> index_list[MAX_OCCURENCE];
    index_out.write(-1);

    wait();
    
    while(1){
        // read input pattern
        m = 0;
        index_out.write(-1); //just tell it's not a valid output
        wait();
        for(i=0; i<MAX_PAT_LEN; i++){
            in_char = (unsigned char)pat_in.read();
            if(in_char!=0){
                pat[i] = in_char;
                m++;
            }
            else
                break;
            
            wait();
        }

        // find pattern
        search( pat, m, index_list );
        
        // write output indices
        for(i=0; i<MAX_OCCURENCE; i++){
            index_out.write( index_list[i] );
            wait();
        }
        
    }
    
}
    
    
    
    
    
    
    
    
    
    
    