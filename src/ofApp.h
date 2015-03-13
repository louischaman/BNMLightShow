#pragma once

#include "ofMain.h"
/*include Addons-----------------------------------------*/
#include "ofxFft.h"
#include "ofxUI.h"


/*include headers----------------------------------------*/
#include "ofBeatDetectChannel.h"
#include "ofFftOctaveBin.h"
#include "ofBeatDetectPlot.h"

#include "ofToggleLight.h"
#include "ofGreyscaleLight.h"
#include "ofColourLight.h"

#include "lightShowSerial.h"

#include "ofLightShowScene.h"

/*defines------------------------------------------------*/


/*Main class---------------------------------------------*/
class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void exit();

		void FFTSetup(int bufferSize);
		void beatSetup(int bufferSize);
		void setupLights(int nLights);
		void setupBalloonSerial();
		void setupUI();

		void audioIn(float* input, int bufferSize, int nChannels);
		void beatUpdate();
		void updateLights();
		void sendMessages();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void guiEvent(ofxUIEventArgs &e);

/*Objects--------------------------------------------*/
        // Sound
        ofSoundStream soundStream;

        // FFT
        float inputScale;   // hmm
        float maxInput;     // use this to write function setting peak sound levels

        vector<float> audioBins, midBins, outBins;
        ofxFft* fft;
        ofFftOctaveBin* octaveBin;
        ofMutex soundMutex;

        // beat detect
        int noBins;
        vector<float> inBins, avgWeights, sensitivityGrad, sensitivityOffset, beatCut, avgLevels;
        vector<bool> beatFlag;
        vector<ofBeatDetectChannel *> beatDetect;
        ofBeatDetectPlot beatPlot;

        // lights
        int nToggleLights;
        vector<ofToggleLight *> tLight;

        int nGreyscaleLights;
        vector<ofGreyscaleLight *> gLight;

        int nColourLights;
        vector<float> colourDamping, colourMinLevel, colourMaxLevel;
        vector<ofColourLight *> cLight;
        float persistantHue;

        //ui
        ofxUITabBar *beatDetectTabBar;
        vector<ofxUICanvas *> beatGui;

        ofxUITabBar *cLightsTabBar;
        vector<ofxUICanvas *> cLightGui;

        //serial
        lightShowSerial cLightSerial;
        lightShowSerial gLightSerial;

        // light scene script
        ofLightShowScene lightScene;
};

