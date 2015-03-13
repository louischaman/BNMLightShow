/*************************************************************
 * @author  Deniz Erbilgin
 *
 * @brief   Detects if beat has occured on input by comparing
 *          to a threshold based on the moving avg
 * @todo    -sort out sensitivity calculation
 *          -some sort of input scaling?
 *
 ************************************************************/

#include "ofBeatDetectChannel.h"

/*Constructor/destructor------------------------------------*/
ofBeatDetectChannel::ofBeatDetectChannel(){
    instantVol = 0.0;
    volDifference = 0.0;
    avgVol = 0.0;
    avgWeight = 0.0;

    sensitivity = 0.0;
    sensitivityGrad = 0.0;
    sensitivityOffset = 0.0;

    variance = 0.0;
    varianceWeight = 0.0;

    beatCutOff = 0.0;

    threshold = 0.0;

    bOldBeat = false;
    bNewBeat = false;
    bBeatFlag = false;
}

ofBeatDetectChannel::~ofBeatDetectChannel(){}

/**
  * @brief  updates instant volumes and calculates variables
  * @param  input: instant vol
  * @retval none
  */
void ofBeatDetectChannel::update(float _input){
    instantVol = _input;

    //calc vol difference
    volDifference = instantVol-avgVol;
    // calc exponential moving average
    avgVol = avgVol + (avgWeight*volDifference);
    //calc variance
    variance = variance - (avgWeight*variance) + (varianceWeight*volDifference*volDifference);
    //calc sensitivity
    sensitivity = (sensitivityGrad*variance) + sensitivityOffset;
    threshold = avgVol*sensitivity;

    // check for beats  ADD FILTERING
    bNewBeat = checkBeat();
    bBeatFlag = edgeDetect();

}

/**
  * @brief  updates beat detection parameters
  * @param  avgWeight: weighting coefficient for averaging
  *         sensitivtyGrad: gradient for changing sensitivity (the m in y = mx + c)
  *         sensitivityOffset: offset for sensitivity (the c in above)
  *         beatCutOff: Mininum level for beat to be accepted
  * @retval none
  */
void ofBeatDetectChannel::updateParams(float _avgWeight, float _sensitivityGrad, float _sensitivityOffset, float _beatCutOff){
    avgWeight = _avgWeight;
    sensitivityGrad = _sensitivityGrad;
    sensitivityOffset = _sensitivityOffset;
    beatCutOff = _beatCutOff;

    // calculate variance weight
    varianceWeight = avgWeight/(1-avgWeight);
}

/**
  * @brief  returns if beat detected
  * @param  none
  * @retval returns true if beat, else false
  */
bool ofBeatDetectChannel::getBeat(){
    return bBeatFlag;
}

/**
  * @brief  returns average volume
  * @param  none
  * @retval returns float of avg volume
  */
float ofBeatDetectChannel::getAvgVol(){
    return avgVol;
}

/**
  * @brief  returns beat detection threshold
  * @param  none
  * @retval float of threshold
  */
float ofBeatDetectChannel::getThreshold(){
    return threshold;
}

/**
  * @brief  Compares instantaneous volume with beat detection threshold and cut off level
  * @param  none
  * @retval returns true if instantVol is greater than threshold and beatcutoff
  */
bool ofBeatDetectChannel::checkBeat(){
    if((instantVol > (threshold)) && (instantVol > beatCutOff)) return true;
    else return false;
}

/**
  * @brief  detects rising edge on beat detection by comparing current value to previous
  * @param  none
  * @retval true if rising edge, else false
  */
bool ofBeatDetectChannel::edgeDetect(){
    //old version
    if ((!bOldBeat) && bNewBeat){
        bOldBeat = bNewBeat;
        return true;
    } else {
        bOldBeat = bNewBeat;
        return false;
    }
}

/**
  * @brief
  * @param
  * @retval
  */
//void ofBeatDetectChannel::
