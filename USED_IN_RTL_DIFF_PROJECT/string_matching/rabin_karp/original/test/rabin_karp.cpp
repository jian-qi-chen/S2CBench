/* Following program is a C implementation of Rabin Karp 
Algorithm given in the CLRS book */
#include<stdio.h> 

// ABSZ is the number of characters in the input alphabet 
#define ABSZ 256 

/* pat -> pattern 
	txt -> text 
	q -> A prime number 
*/

unsigned char txt[471] = {84, 104, 101, 32, 85, 110, 105, 118, 101, 114, 115, 105, 116, 121, 32, 111, 102, 32, 84, 101, 120, 97, 115, 32, 97, 116, 32, 68, 97, 108, 108, 97, 115, 32, 105, 115, 32, 97, 32, 114, 105, 115, 105, 110, 103, 32, 114, 101, 115, 101, 97, 114, 99, 104, 32, 112, 111, 119, 101, 114, 104, 111, 117, 115, 101, 32, 119, 105, 116, 104, 32, 101, 105, 103, 104, 116, 32, 115, 99, 104, 111, 111, 108, 115, 32, 97, 110, 100, 32, 109, 111, 114, 101, 32, 116, 104, 97, 110, 32, 49, 52, 48, 32, 97, 99, 97, 100, 101, 109, 105, 99, 32, 100, 101, 103, 114, 101, 101, 115, 32, 105, 110, 99, 108, 117, 100, 105, 110, 103, 32, 116, 111, 112, 45, 114, 97, 110, 107, 101, 100, 32, 112, 114, 111, 103, 114, 97, 109, 115, 32, 105, 110, 32, 98, 117, 115, 105, 110, 101, 115, 115, 44, 32, 101, 110, 103, 105, 110, 101, 101, 114, 105, 110, 103, 44, 32, 115, 99, 105, 101, 110, 99, 101, 44, 32, 97, 117, 100, 105, 111, 108, 111, 103, 121, 32, 97, 110, 100, 32, 97, 114, 116, 115, 32, 97, 110, 100, 32, 116, 101, 99, 104, 110, 111, 108, 111, 103, 121, 46, 32, 84, 104, 101, 32, 99, 97, 109, 112, 117, 115, 32, 99, 111, 109, 109, 117, 110, 105, 116, 121, 32, 105, 115, 32, 101, 110, 114, 105, 99, 104, 101, 100, 32, 98, 121, 32, 97, 32, 99, 111, 109, 109, 105, 116, 109, 101, 110, 116, 32, 116, 111, 32, 116, 104, 101, 32, 97, 114, 116, 115, 44, 32, 115, 101, 114, 118, 105, 99, 101, 32, 108, 101, 97, 114, 110, 105, 110, 103, 32, 111, 112, 112, 111, 114, 116, 117, 110, 105, 116, 105, 101, 115, 32, 97, 110, 100, 32, 97, 110, 32, 97, 116, 104, 108, 101, 116, 105, 99, 115, 32, 112, 114, 111, 103, 114, 97, 109, 32, 119, 105, 116, 104, 32, 110, 101, 119, 32, 101, 115, 112, 111, 114, 116, 115, 32, 116, 101, 97, 109, 115, 46, 32, 85, 84, 68, 32, 115, 116, 117, 100, 101, 110, 116, 115, 44, 32, 102, 97, 99, 117, 108, 116, 121, 44, 32, 115, 116, 97, 102, 102, 32, 97, 110, 100, 32, 97, 108, 117, 109, 110, 105, 32, 99, 111, 110, 116, 105, 110, 117, 101, 32, 116, 111, 32, 114, 105, 115, 101, 32, 116, 111, 32, 110, 101, 119, 32, 99, 104, 97, 108, 108, 101, 110, 103, 101, 115, 44, 32, 112, 114, 111, 118, 105, 110, 103, 32, 116, 104, 97, 116, 32, 67, 111, 109, 101, 116, 115, 32, 115, 104, 105, 110, 101, 32, 98, 114, 105, 103, 104, 116, 46 }/* Cyber array = RAM */;

void search(unsigned char pat[], int q) 
{ 
	int M = 3;  // length of pattern
	int N = 471; // length of text 
	int i, j; 
	int p = 0; // hash value for pattern 
	int t = 0; // hash value for txt 
	int h = 1; 

	// The value of h would be "pow(ABSZ, M-1)%q" 
	for (i = 0; i < M-1; i++) 
		h = (h*ABSZ)%q; 

	// Calculate the hash value of pattern and first 
	// window of text 
	for (i = 0; i < M; i++) 
	{ 
		p = (ABSZ*p + pat[i])%q; 
		t = (ABSZ*t + txt[i])%q; 
	} 

	// Slide the pattern over text one by one 
	for (i = 0; i <= N - M; i++) 
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
			if (j == M) 
				printf("Pattern found at index %d \n", i); 
		} 

		// Calculate hash value for next window of text: Remove 
		// leading digit, add trailing digit 
		if ( i < N-M ) 
		{ 
			t = (ABSZ*(t - txt[i]*h) + txt[i+M])%q; 

			// We might get negative value of t, converting it 
			// to positive 
			if (t < 0) 
                t = (t + q); 
		} 
	} 
} 

/* Driver program to test above function */
int main() 
{ 
//	unsigned char txt[472] = "The University of Texas at Dallas is a rising research powerhouse with eight schools and more than 140 academic degrees including top-ranked programs in business, engineering, science, audiology and arts and technology. The campus community is enriched by a commitment to the arts, service learning opportunities and an athletics program with new esports teams. UTD students, faculty, staff and alumni continue to rise to new challenges, proving that Comets shine bright."; 
    unsigned char pat[4] = "and"; 
    
    // get the value of char
    printf("Text char array:\n");
    printf("%d",(int)txt[0]);
    for (int i=1; i<472; i++)
        printf(", %d",(unsigned int)txt[i]);
    printf("\n");
    
    printf("Pattern char array:\n");
    printf("%d",(int)pat[0]);
    for (int i=1; i<4; i++)
        printf(", %d",(unsigned int)pat[i]);
    printf("\n");
	
	int q = 101; // A prime number 
	search(pat, q); 
	return 0; 
} 
