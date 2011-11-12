/***************
ANALYSIS CLASS FOR INPLEMENTING AND DISPLAYING DIFFERENT LIVE VIDEO ALGORITHMS
 //cc. non commercial share alike Tom Schofield & Jamie Allen Source Code licenced under GNU v3 2011
*****************/

#ifndef __analysis__
#define __analysis__

#include "ofMain.h"
#include "ofxQtVideoSaver.h"
#include "ofxXmlSettings.h"
#include "ThreePhaseDecoder.h"


using namespace std;

class analysis{
    
    public:
    
    int myVariable;
        
    void setupAnalysis(int camH, int camW, int analasisTimePass, string whichAnalysis, int whichCodec);//, ofVideoGrabber &grabber);
       
    ////Synth Methods//
    void synthDrawCamRecord(unsigned char * pixels);
    void synthUpdate();
    
    ////Analyse Input Methods//
    void analyseInput(unsigned char * pixels);
    
    ///Display Final Result Methods///
    void displayResult();
    void setupMovie();
    void updatePlayer();

    void setupQuicktimeMovieRecord(int camH, int camW, int codec);
    vector<ofImage> returnFrames();
    
    //analysis specific functions (this makes me think that we should have seperate classes for at least some of these analyses
    float returnGaussian(float x, float spread, float height, float centre, float range);
    void linear(float maxResult, float maxTime, float divisions, bool showGraph);
    float skewNormalDistribution(float x, float spread, float height, float max, float centre, float shiftCentre);
    void squareWave(float maxResult, float maxTime, float divisions, bool showGraph);
    void exponential(float maxResult, float maxTime, bool showGraph);
    void quadratic(float maxResult, float maxTime, float divisions, bool showGraph);
    void setupGraphs(); // not a great/clear name feel free to find a better one ;) 

    ///Other
    int analysisTime;

    float animationTimeLimit;
    float animationTime;
    float lastTime;
    float thisTime;
    float timeDiff;
    float fadeTime;
    
    int numberOfCameraImages;
    
    unsigned char * analysedFrame;
    string dataPathName;
    string whichAnalysis;

    bool newFrame;
    bool gotAllLocalFrames1,gotAllLocalFrames2,gotAllLocalFrames3 ;

    //FOR ANALYSIS
    bool synthesisComplete;
    bool analysed;
    
    //FOR RELAXRATE
    string whichGraph;
    float 	graphCounter;
    float level;
    float limiter;
    bool on;
    int flip;
    bool finishedGraph;
    float maxResultA; 
    float maxTimeA; 
    float divisionsA; 
    bool showGraphA;
    
    //FOR M_CODE
    string morseMessage;
    //these vectors hold the dots and dashes of morse (morseCode)  and its equivalent character (textTranslation)
    vector<string> morseCode;
    vector<string> textTranslation;
    int onCounter;
    int offCounter;
    int speed;
    int pauseBetween;
    void loadMorse();
    void showMorse(string message);
    string translateToMorse(string messageToTranslate);
    
    //FOR SINGLE COLOUR
    int red;
    int green;
    int blue;
    
    //FOR CAM FRAME RATE
    void strobe();
    float getRamp();
    float currentFRate;
    ofxQtVideoSaver movieFromCamera;
    ofImage cameraCapture;
    ofVideoPlayer player;
    
    int camHeight;
    int camWidth;
    string cameraMovieName;
    
    int check;
    int i,j,k;
    float counter2;
    float counter2max;
    float testFloat;
    
    int counter;
    float floatCounter;
    int counterMax;
    int frameCounter;
    int localFrameCounter;
    int scanLinePosition;
    int scanLineWidth;
    int scanLineSpeed;

    int framesPerGreyValue;
    int framesPerColourValue;
    
    ofColor aColour;
    ofColor bColour;
    float rValue;
    float gValue;
    float bValue;
    float cHue;
    float greyValue;
    float oldGreyValue;
    float numberOfGreyLevels;
    
    vector <ofImage> imgs;
    vector <unsigned char *> imgPixels;
    
    
        
};

#endif

