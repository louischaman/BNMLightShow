#pragma once

#include "ofMain.h"

class ofBeatDetectPlot{
public:
    // constructor destructor
    ofBeatDetectPlot();
    ~ofBeatDetectPlot();

    void setup(int noBins);

    void draw(int x, int y, int plotWidth, int plotHeight);

    void setValues(vector<float> instantVol, vector<float> avgVol, vector<float> threshold, vector<float> beatCutOff, vector<bool> beatFlag);

private:
    float dBConversion(float input);

    void drawBar(int barNo);

    // setup values
    int x, y, plotWidth, plotHeight, noBins;

    //scaling
    float scale, postScale;
    int n;

    //plot boxes
    vector<float> plotVol, plotAvg, plotThresh, plotCut;
    vector<int> plotBeat;

};
