# Description: Flashes random greyscale and random colour light on each beat

import math
from openframeworks import *

class myScene(object):
#---------------DO NOT TOUCH THESE VARIABLES----------------------------------#
	def __init__(self, nBins, nTLights, nGLights, nCLights):
		# size of FFT and number of lights - Make these auto aquired
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
			self.gLightD = list(0.0 for i in range(self.nGLights))
			self.gLightB = list(0.0 for i in range(self.nGLights))
		if self.nCLights > 0:
			# damping and HSB values for colour lights
			self.cLightC = list(0 for i in range(self.nCLights))
			self.cLightD = list(0.0 for i in range(self.nCLights))
			self.cLightH = list(0.0 for i in range(self.nCLights))
			self.cLightS = list(1.0 for i in range(self.nCLights))
			self.cLightB = list(0.0 for i in range(self.nCLights))	
		self.setup()
	
#---------------MODIFY AFTER HERE---------------------------------------------#

	# User setup here
	def setup(self):
		for i in range(self.nGLights):
			self.gLightD[i] = 0.01
		for i in range(self.nCLights):
			self.cLightD[i] = 0.2

	# main light script goes in here
	def loop(self, avgVol, var, beatFlag):
		self.clearGreys()
		self.clearColours()

		if beatFlag[1]:
			self.tLightF[int(ofRandom(0, self.nTLights))] = True
			self.flashGrey()
			self.flashColour()

#---------------ADD USER FUNCTIONS HERE---------------------------------------#
	# add custom functions here 
	def clearGreys(self):
		for i in range(self.nGLights):
			self.gLightC[i] = 0

	def clearColours(self):
		for i in range(self.nCLights):
			self.cLightC[i] = 0

	def flashGrey(self):
		randomLight = int(ofRandom(0, self.nGLights))
		self.gLightC[randomLight] = 2
		self.gLightB[randomLight] = 1.0

	def flashColour(self):
		randomLight = int(ofRandom(0, self.nCLights))
		self.cLightC[randomLight] = 2
		self.cLightH[randomLight] = ofRandom(0.0, 1.0)
		self.cLightB[randomLight] = 1.0

