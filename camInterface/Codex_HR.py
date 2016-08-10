# coding: utf-8
from __future__ import division
import sys
import csv
import numpy as np
from numpy.linalg import inv
import matplotlib as mpl
import matplotlib.pyplot as plt
import scipy
from scipy import signal
# Command for plotting inside iPython 
#get_ipython().magic(u'matplotlib inline')
from scipy import sparse 
from scipy.sparse import spdiags

"""Function for non linear signal detrending"""
def det(avg,lamb):
    n=avg.shape
    dtr=np.zeros(shape=(n[0],n[1]))
    T=n[0]
    dim=n[1]
    #I=sparse.eye(T).toarray();
    #D2=spdiags(np.multiply(np.ones((T,1)),[1, -2, 1]).T,[0,1,2],T-2,T).toarray()

    #coefficiente = I-inv(I+np.multiply(lamb^2,(np.dot(D2.T,D2))))
    for i in range(0, dim):
        #dtr[:,i]=np.dot(coefficiente,avg[:,i])
        dtr[:,i]=signal.detrend(avg[:,i])
    return dtr

"""FIR filter function"""
def filt(s,Fc,f1,f2):
    fcut=np.multiply([f1,f2],[2/Fc])
    b=scipy.signal.firwin(127,fcut,window='hamming',pass_zero=False)
    a = np.array([1])
    y=signal.filtfilt(b,a,s,padtype=None)
    return y

# In[12]:

with open(sys.argv[1], 'rb') as csvfile:
    reader = csv.reader(csvfile)
    x=list(reader)
    result=np.array(x).astype('float')
    
# Detrending 
print("Detrending phase")
N=result.shape
output=np.zeros(shape=(N[0],N[1]))
window=300
step=np.linspace(0,window-1,window)
while step[-1]<=N[0]:
    inp=det(result[step.astype(int),:],400)  
    output[step.astype(int),:]=inp
    step=step+window


# In[17]:

# Band pass filter
Fc=60
f1=0.3
f2=8
series=np.zeros(shape=(N[0],N[1]))
for j in range(0, N[1]):
    series[:,j]=filt(output[:,j],Fc,f1,f2)


# In[19]:


f,Pxx=signal.welch(series[:,1],Fc,nperseg=2048)
#plt.plot(f,Pxx)
#plt.xlim([0, 8])
#plt.xlabel('frequency [Hz]')
#plt.ylabel('PSD [V**2/Hz]')
#plt.show()

print("finished!")
# In[ ]:



