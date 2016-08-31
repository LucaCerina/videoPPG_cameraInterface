# coding: utf-8
import numpy as np
from numpy.linalg import inv
from scipy import linalg
from scipy import sparse
from scipy import signal 
from scipy.sparse import spdiags

"""Tarvaynen detrending"""
def createDtrCoeffs(dim,lamb):
	I=sparse.eye(dim).todense()
	D2=spdiags(np.multiply(np.ones((dim,1)),[1, -2, 1]).T,[0,1,2],dim-2,dim).todense()
	coeffs = I-inv(I+(lamb*lamb)*(D2.T*D2))
	return coeffs

def signalDetrend(sigInput,coeffs):
	N=sigInput.shape
	sigOutput=np.zeros(N)
	for i in range(0, N[1]):
		sigOutput[:,i]=np.dot(coeffs,sigInput[:,i])
	return sigOutput

"""Zero Phase PCA"""
def zca(sigInput):
	sphere=2*inv(linalg.sqrtm(np.cov(sigInput.T)))
	#sphere=np.asmatrix(sphere)
	#sig=np.asmatrix(sig)
	#zeroca=sig*sphere
	zeroca = np.dot(sigInput,sphere)    
	return zeroca

"""FIR filter function"""
def signalFilter(s,Fc,f1,f2):
	N = s.shape
	y = np.zeros(N)
	fcut=np.multiply([f1,f2],[2.0/Fc])
	b=signal.firwin(127,fcut,window='hamming',pass_zero=False)
	a = np.array([1])
	for i in range(0,N[1]):
		y[:,i]=signal.filtfilt(b,a,s[:,i],padtype=None)
	return y

"""spectrum HR estimation TODO SNR"""
def signalSNR(sigInput, fSample, window,stepSize):
	N = sigInput.shape
	centralFreq = np.zeros((N[0]/stepSize-(window//stepSize)+1,3))
	freqSpan = 0.2
	SNR = centralFreq
	for i in range(0,N[1]):
		step = np.arange(0,window)
		counter = 0
		while step[-1]<=N[0]:
			f,Pxx = signal.welch(sigInput[step,i],fSample,nperseg=window)
			#find cardiac frequency
			fMax = f[np.where(Pxx==max(Pxx))].astype(float)
			centralFreq[counter,i] = fMax
			#calculate SNR
			if((fMax < 0.5) | (fMax > 2.5)):
				SNR[counter,i] = np.nan
			else:
				f1 = np.where((f<=fMax+freqSpan) & (f>=fMax-freqSpan))[0]
				f2 = np.where((f<=2*fMax+freqSpan) & (f>=2*fMax-freqSpan))[0]
				fUp = np.where(f>=5)[0][0].astype(int)
				Spow = np.trapz(Pxx[f1],f[f1]) + np.trapz(Pxx[f2],f[f2])
				Npow = np.trapz(Pxx[0:f1[0]-1],f[0:f1[0]-1])+np.trapz(Pxx[f1[-1]+1:f2[0]-1],f[f1[-1]+1:f2[0]-1])+np.trapz(Pxx[f2[-1]+1:fUp],f[f2[-1]+1:fUp])
				SNR[counter,i] = 10*np.log(Spow/Npow)
			counter += 1
			step += stepSize
	return centralFreq,SNR
