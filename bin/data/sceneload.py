#test
import sys
sys.path.append("data/scenes")

#import math
from openframeworks import *

class sceneLoad(object):
	def __init__(self):
		#variables
		self.nBins = 0
		self.nTLights = 0
		self.nGLights = 0
		self.nCLights = 0
	
	def setupBins(self, nBins):
		self.nBins = nBins
		self.avgVol = list(1.0 for i in range(nBins))
		self.inBins = list(1.0 for i in range(nBins))
		self.var = list(0.0 for i in range(nBins))
		self.beatFlag = list(False for i in range(nBins))
		
		self.avgVol1 = list(1.0 for i in range(nBins))
		
	def setupLights(self, nTLights, nGLights, nCLights):
		self.nTLights = nTLights
		self.nGLights = nGLights
		self.nCLights = nCLights

	def load(self, scene):
		#import scene and reload to apply any changes
		self.scene = __import__(scene)
		reload(self.scene)
		self.curScene = self.scene.myScene(self.nBins, self.nTLights, self.nGLights, self.nCLights)
	
	def update(self):
		self.curScene.loop(self.inBins, self.avgVol, self.var, self.beatFlag)

	def draw(self):
		x = 0 #place holder. will break if removed
	
	# set variables
	def resetTLightF(self):
		for i in range(self.curScene.nTLights):
			self.curScene.tLightF[i] = False
		
	def setAvgLev(self, avgVol, pos):
		self.avgVol[pos] = avgVol

	def setInsLev(self, instVol, pos):
		self.inBins[pos] = instVol
	
	def setVar(self, var, pos):
		self.var[pos] = var
	
	def setBeats(self, beatFlag, pos):
		self.beatFlag[pos] = beatFlag
	
	#toggle light return functions
	def getTLightF(self, pos):
		return self.curScene.tLightF[pos]

	#greyscale light return functions
	def getGLightC(self, pos):
		return self.curScene.gLightC[pos]

	def getGLightD(self, pos):
		return self.curScene.gLightD[pos]
	
	def getGLightB(self, pos):
		return self.curScene.gLightB[pos]

	#colour light return functions
	def getCLightC(self, pos):
		return self.curScene.cLightC[pos]

	def getCLightD(self, pos):
		return self.curScene.cLightD[pos]
	
	def getCLightH(self, pos):
		return self.curScene.cLightH[pos]

	def getCLightS(self, pos):
		return self.curScene.cLightS[pos]

	def getCLightB(self, pos):
		return self.curScene.cLightB[pos]
