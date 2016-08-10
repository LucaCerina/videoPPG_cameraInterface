
# coding: utf-8

# In[1]:

import numpy as np


# In[2]:

from numpy.linalg import inv


# In[10]:

from scipy import sparse 
from scipy.sparse import spdiags


# In[7]:

def det(avg,lamb):
    n=avg.shape
    dtr=np.zeros(shape=(n[0],n[1]))
    T=n[0]
    dim=n[1]
    I=sparse.eye(T).todense()
    D2=spdiags(np.multiply(np.ones((T,1)),[1, -2, 1]).T,[0,1,2],T-2,T).todense()
    coefficiente = I-inv(I+(lamb*lamb)*(D2.T*D2))
    for i in range(0, dim):
        dtr[:,i]=np.dot(coefficiente,avg[:,i])
    return dtr

