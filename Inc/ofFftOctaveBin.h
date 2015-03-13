#pragma once
#include "ofMain.h"

class ofFftOctaveBin{
public:
    ofFftOctaveBin(int nInputBins);
    ~ofFftOctaveBin();

    void set(vector<float> input);
    void update();
    vector<float> get();

private:
    vector<float> inBins, outBins;
    int nBins;
};
