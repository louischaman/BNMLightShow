//extends ofBaseLight to provide greyscale features
//To do: add enumerated list of commands
#pragma once
#include "ofBaseLight.h"
#include "ofLightChannel.h"

class ofGreyscaleLight: public ofBaseLight{
public:
    //constructor
    ofGreyscaleLight(){};
    //destructor
    ~ofGreyscaleLight(){};

    //interface
    void setBrightness(int command, float level){
        switch (command){
            case 0:
                fadeTo(0.0);
                break;
            case 1:
                fadeTo(level);
                break;
            case 2:
                flashTo(level);
                break;
            default:
                break;
        }
    }
    void setDamping(float level){greyscale.setDamping(level); };
    void setMaxLevel(float level){greyscale.setMaxLevel(level); };
    void setMinLevel(float level){greyscale.setMinLevel(level); }

    int output(){
        // if toggle is true, maps current level between 0 and max level and casts as an int
        if (bState){
            return greyscale.output();
        } else return 0;
    };

    void draw(){
        ofPushStyle();
            if(bState){
                ofFill();
                ofSetColor(output());
                ofRect(xPos, yPos, boxSize, boxSize);
            } else drawLightOff();

            ofNoFill();
            ofSetColor(240);
            ofRect(xPos, yPos, boxSize, boxSize);

            ofSetColor(225);
            ofDrawBitmapString("gScale " + ofToString(lightID,0), xPos, yPos + boxSize+16);
        ofPopStyle();
    };

private:
    void flashTo(float level){greyscale.flashTo(level); };
    void fadeTo(float level){greyscale.fadeTo(level); };
    ofLightChannel greyscale;
};
