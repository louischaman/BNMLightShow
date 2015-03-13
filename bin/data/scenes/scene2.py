# @brief:  lights 0 & 1 track lows, 2 & 3 flash on kick, 4 & 5 flash on snare
#

import math
from openframeworks import *

class myScene(object):
#---------------DO NOT TOUCH __init__----------------------------------#
	def __init__(self, nBins, nTLights, nGLights, nCLights):
		# size of FFT and number of lights
		self.nBins = nBins
		self.nTLights = nTLights
		self.nGLights = nGLights
		self.nCLights = nCLights
		
		if self.nTLights > 0:
			# state of toggle lights
			self.tLightF = list(False for i in range(self.nTLights))
		if self.nGLights > 0:
			# damping and brightness of greyscale light
			self.gLightC = list(0 for i in range(self.nGLights))
			self.gLightD = list(0.1 for i in range(self.nGLights))
			self.gLightB = list(0.0 for i in range(self.nGLights))
		if self.nCLights > 0:
			# damping and HSB values for colour lights
			self.cLightC = list(0 for i in range(self.nCLights))
			self.cLightD = list(0.1 for i in range(self.nCLights))
			self.cLightH = list(0.0 for i in range(self.nCLights))
			self.cLightS = list(0.0 for i in range(self.nCLights))
			self.cLightB = list(0.0 for i in range(self.nCLights))	
		self.setup()
	
#---------------MODIFY AFTER HERE---------------------------------------------#

	# setup function called once on loading script
	def setup(self):
		for i in range(0, 2):
			self.cLightD[i] = 0.05
			self.cLightS[i] = 1.0
		for i in range(2, 6):
			self.cLightD[i] = 0.1
			self.cLightS[i] = 1.0
			self.cLightB[i] = 1.0

		self.bassHue = 0.0

	# loop called once per ofx frame
	def loop(self, avgVol, var, beatFlag):
		snare = self.checkSnare(beatFlag)
		newHue = 0.0
		if beatFlag[1]:
			self.bassHue = ofRandom(0.0, 1.0)
		lowVol = avgVol[0]+avgVol[1]+avgVol[2]
		self.volFade(lowVol);

		if beatFlag[1]:
			newHue = ofRandom(0.0, 1.0)
		self.kickFlash(beatFlag[1], newHue)
		
		if snare:
			newHue = ofRandom(0.0, 1.0)
		self.snareFlash(snare, newHue)

#---------------ADD USER FUNCTIONS HERE---------------------------------------#
	# add custom functions here with correct indentation
	def volFade(self, vol):
		dBvol = self.calcdB(vol)
		bright = ofMap(dBvol, 0.0, 40.0, 0.0, 1.0, True)
		for i in range(0, 2):
			self.cLightC[i] = 1
			self.cLightH[i] = self.bassHue
			self.cLightB[i] = bright

	def kickFlash(self, beatFlag, newHue):
		for i in range(2, 4):
			if beatFlag == False:
				self.cLightC[i] = 0
			else:
				self.cLightC[i] = 2
				self.cLightH[i] = newHue
	
	def snareFlash(self, beatFlag, newHue):
		for i in range(4, 6):
			if beatFlag == False:
				self.cLightC[i] = 0
			else:
				self.cLightC[i] = 2
				self.cLightH[i] = newHue

	def checkSnare(self, beatFlag):
		if (beatFlag[5] & beatFlag[6] & beatFlag[7]):
			return True
		return False

	def calcdB(self, avgVol):
		return 10*math.log10(avgVol)
