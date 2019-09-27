// inverse of matrix using LU decomposition
#include "matinv_lu.h"

void matinv_lu::LU(DATA_TYPE D[4][4])
{
    int i,j,k,m;
    DATA_TYPE x;

    for(k=0;k<=2;k++)
    {
        for(j=k+1;j<=3;j++)
        {
            x=D[j][k]/D[k][k];
            for(i=k;i<=3;i++)
            {  
                D[j][i]=D[j][i]-x*D[k][i];
            }
            D[j][k]=x;
        }
    }

}

void matinv_lu::inverse(DATA_TYPE D[4][4], DATA_TYPE inverse[4][4]) 
{ 
    /* Variable declarations */
    int i,j,m;
    DATA_TYPE d[4];
    Y_TYPE y[4],x;

    /* Call a sub-function to calculate the LU decomposed matrix. Note that 
    we pass the two dimensional array [D] to the function and get it back */
    LU(D);


    /*  TO FIND THE INVERSE */

    /* to find the inverse we solve [D][y]=[d] with only one element in 
    the [d] array put equal to one at a time */

    for(m=0;m<=3;m++)
    {
        d[0]=0.0;d[1]=0.0;d[2]=0.0;d[3]=0.0;
        d[m]=1.0;
        for(i=0;i<=3;i++)
        { 
            x=0.0; 
            for(j=0;j<=i-1;j++)
                x=x+D[i][j]*y[j];
            y[i]=(d[i]-x);
        }

        for(i=3;i>=0;i--)
        { 
            x=0.0; 
            for(j=i+1;j<=3;j++)
                x=x+D[i][j]*inverse[j][m];
            inverse[i][m]=(y[i]-x)/D[i][i];
        }
    }

        
} 



void matinv_lu::inv_main() 
{
    int i,j;
	DATA_TYPE input_matrix[4][4];
	static DATA_TYPE inv[4][4]; // To store inverse of A[][] 

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


















