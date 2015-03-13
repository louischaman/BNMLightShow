#include "ofBaseLight.h"

//----------------------------------------------------------
// constructor
ofBaseLight::ofBaseLight(){
    bRegisteredEvents = false;
    bState = true;
    xPos = 0;
    yPos = 0;
    boxSize = 50;
}
//----------------------------------------------------------
// destructor
ofBaseLight::~ofBaseLight(){
    clear();
}

//----------------------------------------------------------
//INTERFACE
//----------------------------------------------------------
// setup
    // sets x and y coordinates and size of light box
void ofBaseLight::setup(int _xPos, int _yPos, int _boxSize, int _lightID){
    xPos = _xPos;
    yPos = _yPos;
    boxSize = _boxSize;
    lightID = _lightID;

    //set mouse events
    if(!bRegisteredEvents){
        ofRegisterMouseEvents(this);
        bRegisteredEvents = true;
    }
}
//----------------------------------------------------------
// toggleState
    // toggles state
void ofBaseLight::toggleState(){
    bState = !bState;
}
// setState
    // sets state to input
void ofBaseLight::setState(bool _bState){
    bState = _bState;
}
// getState
    // returns bool bState
bool ofBaseLight::bReturnState(){
    return bState;
}

//---------------------------------------------------------------
// MOUSECLICKED
//---------------------------------------------------------------
// static function goes outside class
// detects if box clicked
ofEvent<ofVec2f> ofBaseLight::clickedInsideGlobal = ofEvent<ofVec2f>();

// clear
    // unregisters mouse events
void ofBaseLight::clear(){
    if(bRegisteredEvents){
        ofUnregisterMouseEvents(this);
        bRegisteredEvents = false;
    }
}
// mouse events
void ofBaseLight::mouseMoved(ofMouseEventArgs &args){}
void ofBaseLight::mouseDragged(ofMouseEventArgs &args){}
void ofBaseLight::mousePressed(ofMouseEventArgs &args){}
void ofBaseLight::mouseReleased(ofMouseEventArgs &args){
    if (bInside(args.x, args.y)){
        ofVec2f mousePos = ofVec2f(args.x, args.y);
        ofNotifyEvent(clickedInside, mousePos, this);
        ofNotifyEvent(clickedInsideGlobal, mousePos);

        toggleState();
    }
}
// inside
    // checks if click is inside box
bool ofBaseLight::bInside(float _x, float _y){
    bool bIsInside;

    if(_x > xPos && _x < (xPos+boxSize) && _y > yPos && _y < (yPos+boxSize)) bIsInside = true;
    else bIsInside = false;

    return bIsInside;
}

void ofBaseLight::drawLightOff(){
    ofPushMatrix();
    ofTranslate(xPos, yPos);
    ofPushStyle();
        ofSetColor(200);
        ofLine(0, 0, boxSize, boxSize);
    ofPopStyle();
    ofPopMatrix();
}
