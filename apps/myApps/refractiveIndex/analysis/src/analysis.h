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

    ///Other
   
    int timeLimit;
    int analysisTimer;
    unsigned char * analysedFrame;
        
    string whichAnalysis;

    
    //FOR SYNTH
    bool newFrame;
    
    //FOR ALL ANALYSIS
    bool synthesisComplete;
    bool analysed;
    
    ofxQtVideoSaver movieFromCamera;
    
    ofImage cameraCapture;

    ofVideoPlayer player;
    
    int camHeight;
    int camWidth;
    string cameraMovieName;
    
    int check;
    int i;
    
    //FOR SHADOWSCAPES

    int counter;
    int scanLinePosition;
    int scanLineWidth;
    int scanLineSpeed;
    
    vector <ofImage> imgs;
    vector <unsigned char *> imgPixels;
        
};

#endif

