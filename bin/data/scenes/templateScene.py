#                    THIS IS USING PYTHON 2.7
# @brief:  template lighting scene
# 
# @how_to  -DO NOT TOUCH anything in __init__ and class/function names
#          1. Put any python modules at start
#          2. Read notes
#          3. put setup code in setup() and loop code in loop() (like in arduino!)
#
# @notes   -avgVol, var and beatFlag are loaded into loop()
#          -lights in main app are automatically updated with the values in lists
#           named 'self.xLightY' where:
#                x: t is toggled, g is greyscale and c is colour
#                Y: F is bool flag, D is float damping, H is float hue,
#                   S is float saturation and B is float brightness,
#                   C is command: '0' fades to 0, '1' fades to values, '2' flashes
#                   to values
#          -xLightY arrays are all floats between 0.0 and 1.0
#
# @todo	   -change to python 3
#          -move setup and loop to another module to protect init values?
#          -maybe implement 1/damping to make it more natural
#          -make hue easier to use (scale by 360 to give as angle?)
#                   for now, r = 0.0, g = 0.33, b = 0.67

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

	# setup function called once on loading script
	def setup(self):
		x = 0	# replace with your setup code

	# loop called once per ofx frame
	def loop(self, avgVol, var, beatFlag):
		x = 0	# replace with your loop code

#---------------ADD USER FUNCTIONS HERE---------------------------------------#
	# add custom functions here with correct indentation



