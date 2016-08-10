
# coding: utf-8

# In[1]:

import numpy as np
from numpy.linalg import inv
import scipy


# In[2]:

def zca(sig):
    sphere=2*inv(scipy.linalg.sqrtm(np.cov(sig.T)))
    sphere=np.asmatrix(sphere)
    sig=np.asmatrix(sig)
    zeroca=(sphere*sig.T).T
    
    return zeroca

