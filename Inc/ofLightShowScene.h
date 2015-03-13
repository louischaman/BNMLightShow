#include "ofMain.h"

#include "ofxPython.h"

class ofLightShowScene{
public:
    ofLightShowScene();
    ~ofLightShowScene();

    void setup(int nBins, int nTLights, int nGLights, int nCLights);
    void update();
    void draw();

    void select(int newScene);

    void resetTLights();

    void setAvgLevels(vector<float> avgLevels);
    void setVariance(vector<float> variance);
    void setBeatFlags(vector<bool> beatFlags);

    vector<bool>   getTLightF();

    vector<int>     getGLightC();
    vector<float>  getGLightD();
    vector<float>  getGLightB();

    vector<int>    getCLightC();
    vector<float>  getCLightD();
    vector<float>  getCLightH();
    vector<float>  getCLightS();
    vector<float>  getCLightB();

private:
    int currentScene, nBins, nTLights, nGLights, nCLights;
    vector<string> sceneNames;

    ofxPython python;
    ofxPythonObject scene;

    void setLevel(string pyFunction,vector<float> input);
    void setBool(string pyFunction, vector<bool> input);

    vector<int> getInts(string pyObject, int vecSize);
    vector<float> getLevels(string pyObject, int vecSize);
    vector<bool> getBools(string pyObject, int vecSize);
};
