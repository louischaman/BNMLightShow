# Description: Flashes random greyscale and random colour light on each beat

import math
from openframeworks import *
from random import sample

class myScene(object):
#---------------DO NOT TOUCH THESE VARIABLES----------------------------------#
	def __init__(self, nBins, nTLights, nGLights, nCLights):
		# size of FFT and number of lights - Make these auto aquired
		self.nBins = nBins
		self.nTLights = nTLights
		self.nGLights = nGLights
		self.nCLights = nCLights


	#	self.avgVol1  = list(0.0 for i in range(nBins)
		
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
			self.gLightD[i] = 0.05
		for i in range(self.nCLights):
			self.cLightD[i] = 0.2


		self.avgVol1 = list(1.0 for i in range(self.nBins))
		self.avgVol2 = list(0.0 for i in range(self.nBins))
		self.avgVar1 = list(1.0 for i in range(self.nBins))
		self.a = range(10)
	# main light script goes in here
	def loop(self, inBins, avgVol, var, beatFlag):
		#self.clearGreys()
		self.clearColours()

		for i in range(self.nBins):
			self.gLightC[1] = 2
			self.avgVol1[i] = self.avgVol1[i] + (inBins[i]-self.avgVol1[i])*0.003
			self.avgVol2[i] = self.avgVol2[i] + (inBins[i]-self.avgVol2[i])*0.005
			diff = (self.avgVol2[i]-self.avgVol1[i])*(self.avgVol2[i]-self.avgVol1[i])
			self.avgVar1[i] =  self.avgVar1[i] + (diff-self.avgVar1[i])*0.003
		#print self.avgVar1[0]
		for i in range(self.nGLights):
			j = self.a[i]
			sd = math.sqrt(self.avgVar1[j])
			
			Brite = max(0,(self.avgVol2[j]+0.5*sd-self.avgVol1[j])/sd)
			self.gLightC[i] = 2
			self.gLightB[i] = Brite*0.2
		
		if beatFlag[1]:
			self.a = sample(self.a,len(self.a))
			print self.a
		
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
		self.gLightB[randomLight] = 0.5

	def flashColour(self):
		randomLight = int(ofRandom(0, self.nCLights))
		self.cLightC[randomLight] = 2
		self.cLightH[randomLight] = ofRandom(0.0, 1.0)
		self.cLightB[randomLight] = 1.0

