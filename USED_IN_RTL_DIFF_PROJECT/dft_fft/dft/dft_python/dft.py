#! /usr/bin/env python3
import math
def dft(x):
    N = len(x)
    y = [ [0,0] for i in range(N) ]
    
    for k in range(N):
        for n in range(N):
            y[k][0] += x[n][0]*math.cos(2*math.pi*k*n/N)+x[n][1]*math.sin(2*math.pi*k*n/N)
            y[k][1] += -x[n][0]*math.sin(2*math.pi*k*n/N)+x[n][1]*math.cos(2*math.pi*k*n/N)

    return y
    
    
x =[[8.234343,62.32138],[127.1233,123.7856],[70.35434,0.565605],[178.4523,0.997303],[99.56567,66.34565],[12.46543,0.450004],[216.2343,1.886567],[54.45451,23.16784],[11.89763,208.1233],[43.89651,50.56566],[154.2341,0.122225],[42.89365,0.123231],[62.12333,145.1244],[13.55522,127.8546], [89.13445,4.135112],[67.12333,67.12235]]

y = dft(x)
for i in range(len(y)):
    print(y[i][0], y[i][1])

