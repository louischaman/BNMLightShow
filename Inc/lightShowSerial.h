#pragma once
#include "ofMain.h"

class lightShowSerial{
public:
    lightShowSerial();
    ~lightShowSerial();

    void setup(string comPort, int baudRate, int startChar);
    void send(vector<int> message);
    //void close();

private:
    ofSerial serial;
    int startChar;
};
