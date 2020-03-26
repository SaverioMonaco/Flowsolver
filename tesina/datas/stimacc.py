import math
import numpy as np
import pandas as pd
from statistics import mean
import matplotlib.pyplot as plt

Lato=pd.Series([2,3,4,5,6,7])
Percorsi=pd.Series([2,12,184,8512,1262816,575780564])
Percorsiloge=np.sqrt( (np.log(Percorsi)/np.log(math.e)) )
Percorsilogdieci=(np.log(Percorsi))
Percorsisqrtlogdieci=Percorsilogdieci**(1/2)
plt.plot(Lato,Percorsisqrtlogdieci,'bo')
plt.plot(Lato,Percorsisqrtlogdieci)
plt.xlabel('Lato')
plt.ylabel(r'$\frac{1}{2}Log_{10}P$')

def coefficiente(xu,xd,yu,yd):
    m=(yd-yu)/(xd-xu)
    
    return m

m=(coefficiente(Lato[2],Lato[5],Percorsilogdieci[2],Percorsilogdieci[5] )  )

def quota(xu,xd,yu,yd):
    q=(xd*yu-xu*yd)/(xd-xu)

    return q

q=(quota(Lato[2],Lato[5],Percorsilogdieci[2],Percorsilogdieci[5] )  )

lato=6

print("Per il lato ")
print(lato)
print("il numero di percorsi è")
print( 10**((m*lato+q)**2)  )
plt.show()

