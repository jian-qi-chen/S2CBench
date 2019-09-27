// inverse of matrix using adjoint matrix method
#include "matinv_adj.h"

// Function to get cofactor of A[p][q] in temp[][]. n is current 
// dimension of A[][] 
void matinv_adj::getCofactor(DATA_TYPE A[4][4], DATA_TYPE temp[4][4], int p, int q, int n) 
{ 
	int i = 0, j = 0; 

	// Looping for each element of the matrix 
	for (int row = 0; row < n; row++) 
	{ 
		for (int col = 0; col < n; col++) 
		{ 
			// Copying into temporary matrix only those element 
			// which are not in given row and column 
			if (row != p && col != q) 
			{ 
				temp[i][j++] = A[row][col]; 

				// Row is filled, so increase row index and 
				// reset col index 
				if (j == n - 1) 
				{ 
					j = 0; 
					i++; 
				} 
			} 
		} 
	} 
} 

/* expanded recursive function for finding determinant of matrix. 
n is current dimension of A[][]. */
DET_TYPE matinv_adj::determinant4(DATA_TYPE A[4][4]) 
{ 
	DET_TYPE D = 0; // Initialize result 


	DATA_TYPE temp[4][4]; // To store cofactors 

	int sign = 1; // To store sign multiplier 

	// Iterate for each element of first row 
	for (int f = 0; f < 4; f++) 
	{ 
		// Getting Cofactor of A[0][f] 
		getCofactor(A, temp, 0, f, 4); 
		D += sign * A[0][f] * determinant3(temp); 

		// terms are to be added with alternate sign 
		sign = -sign; 
	} 

	return D; 
}

DET_TYPE matinv_adj::determinant3(DATA_TYPE A[4][4]) 
{ 
	DET_TYPE D = 0; // Initialize result 

	DATA_TYPE temp[4][4]; // To store cofactors 

	int sign = 1; // To store sign multiplier 

	// Iterate for each element of first row 
	for (int f = 0; f < 3; f++) 
	{ 
		// Getting Cofactor of A[0][f] 
		getCofactor(A, temp, 0, f, 3); 
		D += sign * A[0][f] * determinant2(temp); 

		// terms are to be added with alternate sign 
		sign = -sign; 
	} 

	return D; 
}

DET_TYPE matinv_adj::determinant2(DATA_TYPE A[4][4]) 
{ 
	DET_TYPE D = 0; // Initialize result 

	DATA_TYPE temp[4][4]; // To store cofactors 

	int sign = 1; // To store sign multiplier 

	// Iterate for each element of first row 
	for (int f = 0; f < 2; f++) 
	{ 
		// Getting Cofactor of A[0][f] 
		getCofactor(A, temp, 0, f, 2); 
		D += sign * A[0][f] * determinant1(temp); 

		// terms are to be added with alternate sign 
		sign = -sign; 
	} 

	return D; 
} 

DET_TYPE matinv_adj::determinant1(DATA_TYPE A[4][4]) 
{ 
	return A[0][0]; 
} 


// Function to get adjoint of A[4][4] in adj[4][4]. 
void matinv_adj::adjoint(DATA_TYPE A[4][4],DATA_TYPE adj[4][4]) 
{ 

	// temp is used to store cofactors of A[][] 
	int sign = 1;
    DATA_TYPE temp[4][4]; 
    /* Cyber unroll_times = 1 */
	for (int i=0; i<4; i++) 
	{ 
        /* Cyber unroll_times = 2 */
		for (int j=0; j<4; j++) 
		{ 
			// Get cofactor of A[i][j] 
			getCofactor(A, temp, i, j, 4); 

			// sign of adj[j][i] positive if sum of row 
			// and column indexes is even. 
			sign = ((i+j)%2==0)? 1: -1; 

			// Interchanging rows and columns to get the 
			// transpose of the cofactor matrix 
			adj[j][i] = (sign)*(determinant3(temp)); 
		} 
	} 
} 

// Function to calculate and store inverse, returns false if 
// matrix is singular 
void matinv_adj::inverse(DATA_TYPE A[4][4], DATA_TYPE inverse[4][4]) 
{ 
	// Find determinant of A[][] 
	DET_TYPE det = determinant4(A); 

	// Find adjoint 
	DATA_TYPE adj[4][4]; 
	adjoint(A, adj); 

	// Find Inverse using formula "inverse(A) = adj(A)/det(A)" 
	for (int i=0; i<4; i++) 
		for (int j=0; j<4; j++) 
			inverse[i][j] = adj[i][j]/det;
        
} 



void matinv_adj::inv_main() 
{
    int i,j;
	DATA_TYPE input_matrix[4][4];
	DATA_TYPE inv[4][4]; // To store inverse of A[][] 

    wait();
    while(1){
        // read inputs
        for(i=0; i<4; i++){
            for(j=0; j<4; j++){
                input_matrix[i][j] = indata.read();
                wait();
            }
        }
        
        inverse(input_matrix, inv);

        for(i=0; i<4; i++){
            for(j=0; j<4; j++){
                outdata.write( inv[i][j] );
                wait();
            }
        }
    
    
    }

} 


















