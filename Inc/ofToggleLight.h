#pragma once

#include "ofMain.h"
#include "ofBaseLight.h"

class ofToggleLight: public ofBaseLight{
public:
    //constructor
    ofToggleLight(){
        bTriggered = false;
        colour = 0;
    };
    //destructor
    ~ofToggleLight(){};

    //methods
    void draw(){
        ofPushStyle();
            ofFill();
            if(bState){
                if(bTriggered) colour = 250;
                else if (colour > 0) colour -= 10;
                ofSetColor(colour);
                ofRect(xPos, yPos, boxSize, boxSize);
            } else {
                drawLightOff();
                colour = 0;
            }

            ofNoFill();
            ofSetColor(240);
            ofRect(xPos, yPos, boxSize, boxSize);

            ofSetColor(225);
            ofDrawBitmapString("toggle " + ofToString(lightID,0), xPos, yPos + boxSize+16);
        ofPopStyle();
    };

    bool bTriggered;

private:
    int colour;
};
