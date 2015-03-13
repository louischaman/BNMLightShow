/*****************************************************************
 * @author  Deniz Erbilgin
 *
 * @brief   Light show program
 * @todo    Make light position external and write class to handle it
 *          try making lightchannel work with hsb
 *          Add min max slider for light gui
 *          Sort out beat detection draw scaling
 *          Make bin assignment toggles for kick snare and hat
 *          Work out how to do scripting
 *
 ****************************************************************/


#include "ofApp.h"

/**
  * @brief      Init app
  */
void ofApp::setup(){


    // Setup Constants:
        int sampleRate = 44100;
        int bufferSize = 1024;
        inputScale = 200.0;

        nToggleLights = 4;

        nGreyscaleLights = 8;
        float gDamping = 0.1;

        nColourLights = 6;
        float cDamping = 0.1;

        string gLightComPort = " ";
        string cLightComPort = "ttyACM0";
        int baudRate = 19200;
        int startChar = 'l';

    // Set up graphics
        //ofSetVerticalSync(true);
        ofSetFrameRate(60);
        ofEnableSmoothing();
        ofBackground(20);
        ofNoFill();
        ofSetColor(240);

    // run setup functions
    FFTSetup(bufferSize);
    beatSetup(bufferSize);

    // setup lights
    for(int i = 0; i < nToggleLights; i++){
        ofToggleLight *newLight = new ofToggleLight();
        newLight->setup(1000, 200+i*96, 64, i);
        tLight.push_back(newLight);
    }


    for(int i = 0; i < nGreyscaleLights; i++){
        ofGreyscaleLight *newLight = new ofGreyscaleLight();
        newLight->setup(788+(i%2)*96, 200+(i/2)*96, 64, i);
        newLight->setDamping(gDamping);
        gLight.push_back(newLight);
    }

    persistantHue = 0.0;
    for (int i = 0; i < nColourLights; i++){
        ofColourLight *newLight = new ofColourLight();
        newLight->setup(564+(i%2)*96, 200+(i/2)*96, 64, i);
        newLight->setDamping(cDamping);
        cLight.push_back(newLight);
    }

    gLightSerial.setup(gLightComPort, baudRate, startChar);
    cLightSerial.setup(cLightComPort, baudRate, startChar);

    setupUI();

    // setup scene
    lightScene.setup(10, nToggleLights, nGreyscaleLights, nColourLights);

    // Setup sound input    MOVE THIS TO STATE 1 TO ALLOW CHANGING SOUNDCARD
    soundStream.listDevices();
    soundStream.setDeviceID(4);
    soundStream.setup(this, 0, 1, sampleRate, bufferSize, 4);
}

/**
  * @brief  processing is done in here
  */
//--------------------------------------------------------------
void ofApp::update(){
    beatUpdate();
    vector<float> threshold;
    for (int i = 0; i < noBins; i++) threshold.push_back(beatDetect[i]->getThreshold());
    beatPlot.setValues(inBins, avgLevels, threshold, beatCut, beatFlag);

    // update scene
    lightScene.setAvgLevels(avgLevels);
    lightScene.setVariance(threshold);  // this needs changing
    lightScene.setBeatFlags(beatFlag);
    lightScene.resetTLights();
    lightScene.update();

    vector<bool> bFlag;
    bFlag = lightScene.getTLightF();
    for (int i = 0; i < nToggleLights; i++){
        tLight[i]->bTriggered = bFlag[i];
    }

    vector<float> cDamping, hue, sat, bright;
    vector<int> cCommands;
    cCommands = lightScene.getCLightC();
    cDamping = lightScene.getCLightD();
    hue = lightScene.getCLightH();
    sat = lightScene.getCLightS();
    bright = lightScene.getCLightB();
    for (int i = 0; i < nColourLights; i++){
        cLight[i]->setDamping(cDamping[i]);
        cLight[i]->setHsb(cCommands[i], hue[i], sat[i], bright[i]);
    }

    vector<float> gDamping, gBright;
    vector<int> gCommands;
    gCommands = lightScene.getGLightC();
    gBright = lightScene.getGLightB();
    gDamping = lightScene.getGLightD();
    for (int i = 0; i < nGreyscaleLights; i++){
        gLight[i]->setDamping(gDamping[i]);
        gLight[i]->setBrightness(gCommands[i], gBright[i]);
    }
    // send serial message
    sendMessages();
}

/**
  * @brief  Keep only drawing functions in here for smooth performance
  */
//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(240);
    ofRect(16, 16, ofGetWindowWidth()-32, ofGetWindowHeight()-32);
    //plot beatdetect
    ofPushMatrix();
    ofTranslate(96, 32);
        beatPlot.draw(0, 0, 400, 600);

        ofTranslate(452, 0);
        ofRect(0, 0, 192, ofGetWindowHeight()-64);

        ofTranslate(224, 0);
        ofRect(0, 0, 192, ofGetWindowHeight()-64);
    ofPopMatrix();
        for(int i = 0; i < nToggleLights; i++) tLight[i]->draw();
        for(int i = 0; i < nColourLights; i++) cLight[i]->draw();
        for(int i = 0; i < nGreyscaleLights; i++) gLight[i]->draw();
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(),2), ofGetWindowWidth()-96, ofGetWindowHeight()-32);
    lightScene.draw();
}

/*setup functions---------------------------------------------*/
/**
  * @brief  Sets up sound and processing objects
  * @param  bufferSize: length of audio buffer to perform fft on
  * @retval none
  */
void ofApp::FFTSetup(int _bufferSize){
    // Setup FFT first
    fft = ofxFft::create(_bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);

    audioBins.assign(fft->getBinSize(), 0.0);
    midBins.assign(fft->getBinSize(), 0.0);
    outBins.assign(fft->getBinSize(), 0.0);

    octaveBin = new ofFftOctaveBin(_bufferSize);
}

/**
  * @brief  Sets up beat detection
  * @param  bufferSize: size of fft (same as audio buffer)
  * @todo   clean up random vectors if not needed
  */
void ofApp::beatSetup(int _bufferSize){
    noBins = log2(_bufferSize);
    inBins.assign(noBins, 0.0);

    avgWeights.assign(noBins, 0.08);
    sensitivityGrad.assign(noBins, -(0.0002*0.0002)); // sort this shit out :/
    sensitivityOffset.assign(noBins, 1.4);
    beatCut.assign(noBins, 10.0);

    // setup beat detects
    for (int i = 0; i < noBins; i++){
        ofBeatDetectChannel *newBeatDetect = new ofBeatDetectChannel();
        newBeatDetect->updateParams(avgWeights[i], sensitivityGrad[i], sensitivityOffset[i], beatCut[i]);
        beatDetect.push_back(newBeatDetect);
    }

    beatFlag.assign(noBins, false);
    avgLevels.assign(noBins, 1.0);

    // set up plot
    beatPlot.setup(noBins);
}

/**
  * @brief  setup gui
  */
void ofApp::setupUI(){
    // beat detect gui
    beatDetectTabBar = new ofxUITabBar();
    beatDetectTabBar->setPosition(32,32);
    for (int i = 0; i < 10; i++){
        string index = ofToString(i);
        ofxUICanvas* gui = new ofxUICanvas();
        gui->setName("BIN" + index);
        gui->addSpacer();
        //gui->addSlider("Smoothing" + index, 1.0, 0.1, avgWeights[i]);
        //gui->addSlider("Gradient" + index, -1.0, 0.0, sensitivityGrad[i]);
        //gui->addSlider("Offset" + index, 1.0, 2.0, sensitivityOffset[i]);
        gui->addSlider("Cut-off" + index, 1.0, 1000.0, beatCut[i]);
        gui->autoSizeToFitWidgets();
        ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);

        gui->loadSettings("settings-" + ofToString(i) + ".xml");

        beatDetectTabBar->addCanvas(gui);
        beatGui.push_back(gui);
    }
    beatDetectTabBar->loadSettings("settings/", "ui-");

    // coloured lights gui
    colourDamping.assign(nColourLights, 0.1);
    colourMaxLevel.assign(nColourLights, 1.0);
    colourMinLevel.assign(nColourLights, 0.0);

    cLightsTabBar = new ofxUITabBar();
    cLightsTabBar->setPosition(564, 48);
    for (int i = 0; i < nColourLights; i++){
        string index = ofToString(i);
        ofxUICanvas* gui = new ofxUICanvas();
        gui->setName("COLOUR" + index);
        gui->addSpacer();
        gui->addSlider("Damping" + index, 0.0, 1.0, colourDamping[i]);
        gui->addRangeSlider("Min Max Level" + index, 0.0, 1.0, colourMinLevel[i], colourMaxLevel[i]);
        gui->autoSizeToFitWidgets();
        ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);

        cLightsTabBar->addCanvas(gui);
        cLightGui.push_back(gui);
    }
}


/*update functions-------------------------------------------*/
/**
  * @brief  takes audio and puts in fft
  * @todo   add autoscaling function
  */
void ofApp::audioIn(float* input, int bufferSize, int nChannels){
    maxInput = 0.0;

    for (int i = 0; i < bufferSize; i++){
        input[i] *= inputScale;
        if(input[i] > maxInput) maxInput = input[i];
    }
    //run fft
    fft->setSignal(input);
    float* curFft = fft->getAmplitude();
    memcpy(&audioBins[0], curFft, sizeof(float)*fft->getBinSize());

    // copy to mid buffer
    soundMutex.lock();
        midBins = audioBins;
    soundMutex.unlock();
}

/**
  * @brief  post process fft and run beat detection
  * @todo   sort out scaling of outbins
  */
void ofApp::beatUpdate(){
    // get output of fft
    soundMutex.lock();
        outBins = midBins;
    soundMutex.unlock();

    octaveBin->set(outBins);
    octaveBin->update();
    inBins = octaveBin->get();

    // beat detection
    for (int i = 0; i < noBins; i++){
        beatDetect[i]->update(inBins[i]);

        beatFlag[i] = beatDetect[i]->getBeat();
        avgLevels[i] = beatDetect[i]->getAvgVol();
    }
}

/**
  * @brief  send serial messages
  */
void ofApp::sendMessages(){
    // put light intensities into a vector
    vector<int> lightMessage;
    for (int i = 0; i < nColourLights; i++){
        vector<int> rgb = cLight[i]->output();
        for (int j = 0; j < 3; j++) lightMessage.push_back(rgb[j]);
    }

    // send message
    cLightSerial.send(lightMessage);
}

/**
  *
  */
//void ofApp::

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    if (key >= '0' && key <= '9'){
        lightScene.select(key-'0');
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    /*if (scene){
		ofxPythonObject at = scene.attr("mouseMoved");
		if (at)
			at( ofxPythonObject::fromInt(x),
				ofxPythonObject::fromInt(y));
	}*/
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
//-----------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
//    int kind = e.widget->getKind();

    string guiName = e.widget->getCanvasParent()->getName();
    int index = guiName[guiName.length()-1]-'0';
    /*if(kind == OFX_UI_WIDGET_TOGGLE){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        int toggleNo = (toggle->getID()/2)-2;
    }*/


    /*if(name == "Smoothing" + ofToString(index)){
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            avgWeights[index] = (slider->getScaledValue())/10;
            beatDetect[index]->updateParams(avgWeights[index], sensitivityGrad[index], sensitivityOffset[index], beatCut[index]);
    } else if(name == "Gradient" + ofToString(index)){
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            float value = slider->getScaledValue()/1000;
            sensitivityGrad[index] = value * value;
            beatDetect[index]->updateParams(avgWeights[index], sensitivityGrad[index], sensitivityOffset[index], beatCut[index]);
    }else if(name == "Offset" + ofToString(index)){
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            sensitivityOffset[index] = slider->getScaledValue();
            beatDetect[index]->updateParams(avgWeights[index], sensitivityGrad[index], sensitivityOffset[index], beatCut[index]);
    }else */if(name == "Cut-off" + ofToString(index)){
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            beatCut[index] = slider->getScaledValue();
            beatDetect[index]->updateParams(avgWeights[index], sensitivityGrad[index], sensitivityOffset[index], beatCut[index]);
    }else if(name == "Damping" + ofToString(index)){
            ofxUISlider *slider = (ofxUISlider *) e.widget;
            cLight[index]->setDamping(slider->getScaledValue());
    }else if(name == "Min Max Level" + ofToString(index)){
            ofxUIRangeSlider *slider = (ofxUIRangeSlider *) e.widget;
            cLight[index]->setMaxLevel(slider->getScaledValueHigh());
            cLight[index]->setMinLevel(slider->getScaledValueLow());
    }

}
//------------------------------------------------------------
void ofApp::exit(){
    int guiNo = 0;
    for(vector<ofxUICanvas *>::iterator it = beatGui.begin(); it != beatGui.end(); ++it){
        ofxUICanvas *g = *it;

        g->saveSettings("settings-" + ofToString(guiNo) + ".xml");

        delete g;
        guiNo++;
    }
    delete beatDetectTabBar;

    for(vector<ofxUICanvas *>::iterator it = cLightGui.begin(); it != cLightGui.end(); ++it){
        ofxUICanvas *g = *it;
        delete g;
    }
    delete cLightsTabBar;
}

