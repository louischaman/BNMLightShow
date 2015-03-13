// extends ofBaseLight to provide colour
#pragma once
#include "ofBaseLight.h"
#include "ofLightChannel.h"

class ofColourLight: public ofBaseLight{
public:
    ofColourLight(){};
    ~ofColourLight(){};

    //interface
    void setHsb(int command, float hue, float saturation, float brightness){
        switch (command){
            case 0:
                fadeToHsb(0.0, 0.0, 0.0);
                break;
            case 1:
                fadeToHsb(hue, saturation, brightness);
                break;
            case 2:
                flashToHsb(hue, saturation, brightness);
                break;
            default:
                break;
        }
    }


    void flashToRgb(float red, float green, float blue){
        colour.set(red, green, blue);
        flashTo();
    }

    void flashToHsb(float hue, float saturation, float brightness){
        colour.setHsb(hue, saturation, brightness);
        flashTo();
    }

    void fadeToRgb(float red, float green, float blue){
        colour.set(red, green, blue);
        fadeTo();
    }

    void fadeToHsb(float hue, float saturation, float brightness){
        colour.setHsb(hue, saturation, brightness);
        fadeTo();
    }

    void setDamping(float level){
        redLight.setDamping(level);
        greenLight.setDamping(level);
        blueLight.setDamping(level);
    }

    void setMaxLevel(float level){
        redLight.setMaxLevel(level);
        greenLight.setMaxLevel(level);
        blueLight.setMaxLevel(level);
    }

    void setMaxLevel(float red, float green, float blue){
        redLight.setMaxLevel(red);
        greenLight.setMaxLevel(green);
        blueLight.setMaxLevel(blue);
    }

    void setMinLevel(float level){
        redLight.setMinLevel(level);
        greenLight.setMinLevel(level);
        blueLight.setMinLevel(level);
    }

    void setMinLevel(float red, float green, float blue){
        redLight.setMinLevel(red);
        greenLight.setMinLevel(green);
        blueLight.setMinLevel(blue);
    }

    vector<int> output(){
        vector<int> rgb;
        rgb.assign(3, 0);
        if (bState){
            rgb[0] = redLight.output();
            rgb[1] = greenLight.output();
            rgb[2] = blueLight.output();
        }
        return rgb;
    }

    void draw(){
        vector<int> rgb = output();
        ofPushStyle();
            if (bState){
                ofFill();
                ofSetColor(rgb[0], rgb[1], rgb[2]);
                ofRect(xPos, yPos, boxSize, boxSize);
            } else drawLightOff();

            ofNoFill();
            ofSetColor(240);
            ofRect(xPos, yPos, boxSize, boxSize);

            ofSetColor(225);
            ofDrawBitmapString("Colour " + ofToString(lightID,0), xPos, yPos + boxSize+16);
        ofPopStyle();
    }

private:
    void flashTo(){
        redLight.flashTo(colour.r);
        greenLight.flashTo(colour.g);
        blueLight.flashTo(colour.b);
    }
    void fadeTo(){
        redLight.fadeTo(colour.r);
        greenLight.fadeTo(colour.g);
        blueLight.fadeTo(colour.b);
    }
    // channels
    ofLightChannel redLight;
    ofLightChannel greenLight;
    ofLightChannel blueLight;

    ofFloatColor colour;
};
