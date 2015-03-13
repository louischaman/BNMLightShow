#pragma once

#include "ofMain.h"

class ofBaseLight{
public:
    //constructor
    ofBaseLight();
    //destructor
    ~ofBaseLight();

    //methods
    void setup(int xPos, int yPos, int boxSize, int lightID);
    void toggleState();
    void setState(bool _bState);

    bool bReturnState();

    virtual void draw() = 0;

        // mouse clicked
        ofEvent<ofVec2f> clickedInside;
        static ofEvent<ofVec2f> clickedInsideGlobal;
        void mouseMoved(ofMouseEventArgs & args);
        void mouseDragged(ofMouseEventArgs & args);
        void mousePressed(ofMouseEventArgs & args);
        void mouseReleased(ofMouseEventArgs & args);
        bool bInside(float _x, float _y);

protected:
    bool bRegisteredEvents;
    void clear();
    void drawLightOff();
    bool bState;
    int xPos, yPos, boxSize, lightID;
private:


};
