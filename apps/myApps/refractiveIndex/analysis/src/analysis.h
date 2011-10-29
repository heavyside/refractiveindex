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
        
    void setupAnalysis(int camH, int camW, int timeLimiter, string whichAnalysis);//, ofVideoGrabber &grabber);
    void runAnalysis(unsigned char * pixels);
    void synthDraw();
    void displayResult();
    void setupMovie();
    void updatePlayer();
    void captureFromInput();
    void setupSaver(int camH, int camW, int codec);
    void saveOut();
    vector<ofImage> returnFrames();
    
    int counter;
    int timeLimit;
    int analysisTimer;
    unsigned char * analysedFrame;
        
    string whichAnalysis;

    //FOR ALL ANALYSIS
    bool synthesisComplete;
    bool analysed;
    
    ofxQtVideoSaver saver;
    ofVideoPlayer player;
    int camHeight;
    int camWidth;
    string movieName;
    
    int check;
    
    //FOR H_SHADOWSCAPES
    int scanLineWidth;
    
    
};

#endif

