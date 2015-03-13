/****************************************************************
 *
 * @author  Deniz Erbilgin
 *
 * @brief   Takes raw fft data, converts to power scale and outputs
 *          in octave bins
 *
 ***************************************************************/

 #include "ofFftOctaveBin.h"

ofFftOctaveBin::ofFftOctaveBin(int _nInputBins){
    nBins = _nInputBins;
    inBins.assign(nBins, 0.0);

    nBins = log2(nBins);
    outBins.assign(nBins, 0.0);

}

ofFftOctaveBin::~ofFftOctaveBin(){}

void ofFftOctaveBin::set(vector<float> input){
    inBins = input;
}

vector<float> ofFftOctaveBin::get(){
    return outBins;
}

void ofFftOctaveBin::update(){
    // scale and convert to power
    for (unsigned int i = 0; i < inBins.size(); i++){
        inBins[i] *= 10.0;
        inBins[i] *= inBins[i];
    }

    // collect fft bins into bins of 1 octave each
    //int numCounted = 0;
    for (int i = 0; i < nBins; i++){
        int numCounted = 0;
        outBins[i] = 0;
        if(i){
            for(int j = pow(2, (i-1)); j < pow(2,i); j++){
                outBins[i] += inBins[j];
                numCounted++;
            }
            inBins[i] /= static_cast<float>(numCounted);
        } else outBins[0] = inBins[0];
    }
}
