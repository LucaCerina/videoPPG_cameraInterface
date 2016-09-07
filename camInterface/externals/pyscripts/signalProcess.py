# coding: utf-8
from __future__ import division
import sys
import csv
import numpy as np
import matplotlib.pyplot as plt
# Command for plotting inside iPython 
#get_ipython().magic(u'matplotlib inline')
import library as clb
import ampdLib
from scipy.io import loadmat

# Read signal file
print("File opening")
with open(sys.argv[1], 'r') as csvfile:
	reader = csv.reader(csvfile)
	x = list(reader)
	result = np.array(x).astype('float')
#Load sampling rate
if(sys.argv[1].find("rest") != -1):
	matRecord = loadmat(sys.argv[2], variable_names = "effectiveFrameRate_rest")
	Fc = matRecord['effectiveFrameRate_rest']
else:
	matRecord = loadmat(sys.argv[2], variable_names = "effectiveFrameRate_stand")
	Fc = matRecord['effectiveFrameRate_stand']
Fc = float(Fc[0])
#Fc = float(sys.argv[2])


# Detrending phase
print("Detrending phase")
N = result.shape
sigDtr = np.zeros(N)
window = 300
step = np.arange(0, window)
dtrCoeffs = clb.createDtrCoeffs(window, 400)
while step[-1] <= N[0]:
	sigDtr[step,:] = clb.signalDetrend(result[step,:], dtrCoeffs)
	step = step + window

#BandPass filter
print("Filtering phase")
f1 = 0.3
f2 = 8
sigFilt = clb.signalFilter(sigDtr, Fc, f1, f2)

# ZCA
print("ZCA phase")
N = sigFilt.shape
sigZca = np.zeros(N)
window = 300;
for i in range(0, 3):
	step = np.arange(0, window)
	while step[-1] <= N[0]:
		sigZca[step, i*3:i*3+3] = clb.zca(sigFilt[step, i*3:i*3+3])
		step = step + window
sigZca = sigZca[:, np.arange(1, 8, 3)] #extract only green channel

# SNR identification
centralFreq, SNR = clb.signalSNR(sigZca, Fc, 600, 150)

#print(centralFreq)
#print(SNR)

# Output the clean signal
print("Signal save")
outFile = sys.argv[1]
outFile = outFile[:outFile.find("_rgb")] + "_clean.csv"
np.savetxt(outFile, sigZca, delimiter=",", fmt="%.6f")

# Find peaks
print("Peak identification")
snrMax = np.sum(SNR, 0)
index = np.flatnonzero(snrMax == np.max(snrMax))[0] # Highest SNR
print("\t Selected signal " + str(index))
pkPos = ampdLib.ampdFast(sigZca[:, index], 10)

#plt.plot(sigFilt[:,1])
plt.plot(sigZca[:, index])
plt.plot(pkPos, sigZca[pkPos, index],'ro')
plt.show()
