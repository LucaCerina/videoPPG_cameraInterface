# -*- coding: utf-8 -*-
"""
Created on Tue Sep  6 11:04:21 2016

@author: exeless
"""

import numpy as np
#import matplotlib.pyplot as plt
import time

"""AMPD function"""
def ampd(sigInput):
	# Create preprocessing fit	
	# sigTime = np.arange(0, len(sigInput))
	
	# fitPoly = np.polyfit(sigTime, sigInput, 1) 
	# sigFit = np.polyval(fitPoly, sigTime) 
	
	# Detrend
	# dtrSignal = sigInput - sigFit 
	
	N = len(sigInput) 
	L = int(np.ceil(N / 2.0)) -1
	
	# Generate random matrix
	LSM = np.random.uniform(1.0, 2.0, size = (L,N)) #uniform + alpha=1

	
	# Local minima extraction
	for k in range(1, L, 1):
		for i in range(k+1, N-k):
			if((sigInput[i-1]>sigInput[i-k-1]) 
			& (sigInput[i-1]>sigInput[i+k-1])):
				LSM[k-1, i-1] = 0 

	#plt.imshow(LSM, extent=[0,100,0,1], aspect=100)
	#plt.show()	
	
	# Find minima				
	G = np.sum(LSM, 1) 
	l = np.flatnonzero(G == G.min())[0]
	
	LSM = LSM[0:l, :]
	
	S = np.std(LSM, 0)
	
	return np.flatnonzero(S == 0)	
		
def ampdFast(sigInput, order):
	"""Accelerate AMPD by signal subdivision."""
	
	# Check if order is valid
	if(len(sigInput)%order != 0):
		print("AMPD: Invalid order, decreasing order")
		while(len(sigInput)%order != 0):
			order -= 1
		print("AMPD: Using order " + str(order))

	N = int(len(sigInput) / order / 2)

	# Loop function calls
	for i in range(0, len(sigInput)-N, N):
		pksTemp = ampd(sigInput[i:(i+2*N-1)])
		if(i == 0):
			pks = pksTemp
		else:
			pks = np.concatenate((pks,pksTemp+i))
		
	# Keep only unique values
	pks = np.unique(pks)
	
	return pks
		