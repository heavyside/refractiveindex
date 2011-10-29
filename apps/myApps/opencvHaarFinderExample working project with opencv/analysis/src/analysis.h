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
#include "ofx3DUtils.h"



using namespace std;


class analysis{
    
    public:
    
    int myVariable;
    bool analysed;
    
    void setupAnalysis(int camH, int camW, int timeLimiter, int whichAnalysis);//, ofVideoGrabber &grabber);
    void runAnalysis(unsigned char * pixels);
    void displayProcess();
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
    
    bool usesMovie;
    
    int whichAnalysis;
    //FOR THREE PHASE STRUCTURED LIGHT
    GLfloat* vertices;
    GLubyte* colors ;
    ThreePhaseDecoder threePhase;
    int vCount;
    bool finishedDisplayingStripes;
    ofImage stripe1, stripe2, stripe3;
    
    ofImage phase1, phase2, phase3;

    
    //FOR ALL ANALYSIS
    ofxQtVideoSaver saver;
    ofVideoPlayer player;
    int camHeight;
    int camWidth;
    string movieName;
    
    //FOR 3D ANALYSIS WITH GL
    ofxCamera camera;
    float lerpZ;
    float centerX, centerY, centerZ;
	float rotX, rotY;
	
	bool bSmoothLight;
	float camX, camY, camZ;
    int check;
};

#endif

