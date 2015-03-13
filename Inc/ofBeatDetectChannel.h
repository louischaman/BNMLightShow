#pragma once
#include "ofMain.h"

class ofBeatDetectChannel{
public:
    //constructor destructor
    ofBeatDetectChannel();
    ~ofBeatDetectChannel();

    void update(float input);
    void updateParams(float _avgWeight, float _sensitivityGrad, float _sensitivityOffset, float _beatCutOff);

    bool getBeat();

    float getAvgVol();
    float getThreshold();

private:
    //private functions
    bool checkBeat();
    bool edgeDetect();

    float input;
    float instantVol, avgVol, volDifference, avgWeight;
    float sensitivity, sensitivityGrad, sensitivityOffset;
    float varianceWeight, variance;
    float beatCutOff;
    float threshold;
    bool bOldBeat, bNewBeat, bBeatFlag;

};
