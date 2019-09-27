#! /usr/bin/env python3
import numpy as np
input_matrix = np.array([[1.199219,1.898438,-2.898438,4.000000],[0.699219,-7.699219,0.328125,1.398438],[-0.851562,-2.199219,0.191406,0.398438],[5.500000,7.800781,-0.101562,0.500000]])
m_det = np.linalg.det( input_matrix )
inv_matrix = np.linalg.inv( input_matrix )

print('input matrix = \n', input_matrix,'\n')
print('determinant =', m_det)
print('inverse = \n', inv_matrix)
