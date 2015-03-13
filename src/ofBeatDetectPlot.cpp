/***********************************************************
 * @author  Deniz Erbilgin
 *
 * @brief   Plots fft and beat detection values
 * @todo    Sort out decibel scaling issues
 *          find better way of representing threshold and avg
 *          sort out kick snare hat writing
 *          sort out labels
 ***********************************************************/


#include "ofMain.h"

#include "ofBeatDetectPlot.h"

ofBeatDetectPlot::ofBeatDetectPlot(){

}

ofBeatDetectPlot::~ofBeatDetectPlot(){}

void ofBeatDetectPlot::setup(int _noBins){
    noBins = _noBins;

    scale = 1000000.0;
    postScale = 10.0;

    plotVol.assign(noBins, 0.0);
    plotAvg.assign(noBins, 0.0);
    plotThresh.assign(noBins, 0.0);
    plotCut.assign(noBins, 0.0);

    plotBeat.assign(noBins, 0);
}

void ofBeatDetectPlot::draw(int x, int y, int _plotWidth, int _plotHeight){
    plotWidth = _plotWidth;
    plotHeight = _plotHeight;
    n = plotWidth/noBins;

    ofPushStyle();
    ofPushMatrix();
    ofTranslate(x, y);
        ofPushMatrix();
        ofTranslate(0, plotHeight);

            // plot bars and bar labels
            for (int i = 0; i < noBins; i++){
                drawBar(i);
            }
        ofPopMatrix();

        // plot label and frame
        ofDrawBitmapString("Spectrum", 16, 16);
        if(plotBeat[1]) ofDrawBitmapString("kick", 32, 64);
        if(plotBeat[5] && plotBeat[6] && plotBeat[7]) ofDrawBitmapString("snare", 32, 96);
        if(plotBeat[8] && plotBeat[9]) ofDrawBitmapString("hat", 32, 128);

        ofRect(0, 0, plotWidth, plotHeight);
    ofPopMatrix();
    ofPopStyle();
}

void ofBeatDetectPlot::setValues(vector<float> instantVol, vector<float> avgVol, vector<float> threshold, vector<float> beatCutOff, vector<bool> beatFlag){
    for(int i = 0; i < noBins; i++){
        plotVol[i] = dBConversion(instantVol[i]);
        plotAvg[i] = dBConversion(avgVol[i]);
        plotThresh[i] = dBConversion(threshold[i]);
        plotCut[i] = -dBConversion(beatCutOff[i]);

        if(plotBeat[i] > 0){
                plotBeat[i]--;
        }
        if(beatFlag[i]) plotBeat[i] = 10;
    }
}

float ofBeatDetectPlot::dBConversion(float input){
    float output = input * scale;
    output = 10*log10f(output);
    output -= 60.0;
    output *= postScale;
    return output;
}

void ofBeatDetectPlot::drawBar(int barNo){
    int i = barNo;
    ofPushStyle();
        ofFill();

        ofSetColor(0);
        if (plotAvg[i] > 0.0) ofRect(i*n+1, 0, n-3, -plotAvg[i]);
        if (plotThresh[i] > plotAvg[i]) ofSetColor(30, 50 + plotBeat[i]*plotBeat[i], 30);
        else ofSetColor(50 + plotBeat[i]*plotBeat[i], 30, 30);
        if (plotThresh[i] > 0.0) ofRect(i*n, -plotAvg[i], n-1, plotAvg[i]-plotThresh[i]);

        ofNoFill();
        ofSetColor(225);
        if (plotVol[i] > 0.0) ofRect(i*n, 0, n-1, -plotVol[i]);

        ofSetLineWidth(2);
        ofLine(i*n, plotCut[i], (i+1)*n, plotCut[i]);
    ofPopStyle();
}
