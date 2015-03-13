#pragma once
#include "ofMain.h"

class ofLightChannel{
public:
    //constructor destructor
    ofLightChannel();
    ~ofLightChannel();

    //methods
    void flashTo(float level);
    void fadeTo(float level);
    void setDamping(float damping);
    void setMaxLevel(float level);
    void setMinLevel(float level);

    int output();

private:
    void updateLevel();

    //variables
    float targetLevel;
    float currentLevel;
    float maxLevel;
    float minLevel;

    int outputLevel;
    float damping;

};
