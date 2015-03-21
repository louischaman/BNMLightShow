/***
 * @brief   scene stuff
 * @todo    figure out how to pass vectors to python
 */

#include "ofLightShowScene.h"
/*Constructor destructor-----------------------------------------------------*/
ofLightShowScene::ofLightShowScene(){
    currentScene = 1;
    string nameProto = "scene";
    for(int i = 0; i < 10; i++){
        string newName = nameProto + ofToString(i);
        sceneNames.push_back(newName);
    }
    python.init();
}

ofLightShowScene::~ofLightShowScene(){}

/*Setup stuff-----------------------------------------------------*/
void ofLightShowScene::setup(int _nBins, int _nTLights, int _nGLights, int _nCLights){
    nBins = _nBins;
    nTLights = _nTLights;
    nGLights = _nGLights;
    nCLights = _nCLights;
    python.executeScript("sceneload.py");
    ofxPythonObject object = python.getObject("sceneLoad");
    if(object) scene = object();
    if(scene){
        ofxPythonObject at = scene.attr("setupBins");
        if(at) at(ofxPythonObject::fromInt(nBins));

        at = scene.attr("setupLights");
        if(at) at(ofxPythonObject::fromInt(nTLights),
                  ofxPythonObject::fromInt(nGLights),
                  ofxPythonObject::fromInt(nCLights));
    }
    select(1);
}

/*Public functions for updating/drawing-----------------------------------------------------*/
void ofLightShowScene::update(){
    if(scene){
        ofxPythonObject at = scene.attr("update");
        if (at) at();
    }
}

void ofLightShowScene::draw(){
    ofDrawBitmapString(sceneNames[currentScene], ofGetWindowWidth()-96, ofGetWindowHeight()-48);
    /*if(scene){
        ofxPythonObject at = scene.attr("draw");
        if (at) at();
    }*/
}
/*Public func for choosing scene-----------------------------------------------------*/
void ofLightShowScene::select(int _newScene){
    currentScene = _newScene;

    if(scene){
        ofxPythonObject at = scene.attr("load");
        if (at) at(ofxPythonObject::fromString(sceneNames[currentScene]));
    }
}
/*Public func for setting beat detect values to python scripts-----------------------------------------------------*/
void ofLightShowScene::resetTLights(){
    if(scene){
        ofxPythonObject at = scene.attr("resetTLightF");
        if (at) at();
    }
}

void ofLightShowScene::setAvgLevels(vector<float> avgLevels){
    vector<float> input = avgLevels;
    if(scene){
        setLevel("setAvgLev", input);
    }
}

void ofLightShowScene::setInsLevels(vector<float> insLevels){
    vector<float> input = insLevels;
    if(scene){
        setLevel("setInsLev", input);
    }
}

void ofLightShowScene::setVariance(vector<float> variance){
    vector<float> input = variance;
    if(scene){
        setLevel("setVar", input);
    }
}

void ofLightShowScene::setBeatFlags(vector<bool> beatFlags){
    vector<bool> input = beatFlags;
    if(scene){
        setBool("setBeats", input);
    }
}
/*Public func for getting light values from scripts-----------------------------------------------------*/
vector<bool> ofLightShowScene::getTLightF(){
    if(scene){
        vector<bool> flags;
        flags = getBools("getTLightF", nTLights);
        return flags;
    }
}

vector<int> ofLightShowScene::getGLightC(){
    if(scene){
        vector<int> commands;
        commands = getInts("getGLightC", nGLights);
        return commands;
    }
}

vector<float> ofLightShowScene::getGLightD(){
    if(scene){
        vector<float> levels;
        levels = getLevels("getGLightD", nGLights);
        return levels;
    }
}

vector<float> ofLightShowScene::getGLightB(){
    if(scene){
        vector<float> levels;
        levels = getLevels("getGLightB", nGLights);
        return levels;
    }
}

vector<int> ofLightShowScene::getCLightC(){
    if(scene){
        vector<int> commands;
        commands = getInts("getCLightC", nCLights);
        return commands;
    }
}

vector<float> ofLightShowScene::getCLightD(){
    if(scene){
        vector<float> levels;
        levels = getLevels("getCLightD", nCLights);
        return levels;
    }
}

vector<float> ofLightShowScene::getCLightH(){
    if(scene){
        vector<float> levels;;
        levels = getLevels("getCLightH", nCLights);
        return levels;
    }
}

vector<float> ofLightShowScene::getCLightS(){
    if(scene){
        vector<float> levels;
        levels = getLevels("getCLightS", nCLights);
        return levels;
    }
}

vector<float> ofLightShowScene::getCLightB(){
    if(scene){
        vector<float> levels;
        levels = getLevels("getCLightB", nCLights);
        return levels;
    }
}

/*private functions-----------------------------------------------------*/
void ofLightShowScene::setLevel(string pyFunction, vector<float> input){
    ofxPythonObject at = scene.attr(pyFunction);
    if (at){
        for(unsigned int i = 0; i < input.size(); i++){
            at( ofxPythonObject::fromFloat(input[i]),
                ofxPythonObject::fromInt(i));
        }
    }
}

void ofLightShowScene::setBool(string pyFunction, vector<bool> input){
    ofxPythonObject at = scene.attr(pyFunction);
    if (at){
        for(unsigned int i = 0; i < input.size(); i++){
            at( ofxPythonObject::fromBool(input[i]),
                ofxPythonObject::fromInt(i));
        }
    }
}

vector<float> ofLightShowScene::getLevels(string pyObject, int vecSize){
    if(scene){
        ofxPythonObject at = scene.attr(pyObject);
        if (at){
            vector<float> levels;
            for (int i = 0; i < vecSize; i ++){
                ofxPythonObject x = at(ofxPythonObject::fromInt(i));
                levels.push_back(x.asFloat());
            }
            return levels;
        }
    }
}

vector<int> ofLightShowScene::getInts(string pyObject, int vecSize){
    if(scene){
        ofxPythonObject at = scene.attr(pyObject);
        if (at){
            vector<int> ints;
            for (int i = 0; i < vecSize; i ++){
                ofxPythonObject x = at(ofxPythonObject::fromInt(i));
                ints.push_back(x.asInt());
            }
            return ints;
        }
    }
}

vector<bool> ofLightShowScene::getBools(string pyObject, int vecSize){
    if(scene){
        ofxPythonObject at = scene.attr(pyObject);
        if (at){
            vector<bool> flags;
            for (int i = 0; i < vecSize; i ++){
                ofxPythonObject x = at(ofxPythonObject::fromInt(i));
                flags.push_back(x.asBool());
            }
            return flags;
        }
    }
}
