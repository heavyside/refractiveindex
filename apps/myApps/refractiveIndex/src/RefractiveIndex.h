/*
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ~ author: dviid
 ~ contact: dviid@labs.ciid.dk 
 */

#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"

#include "AbstractAnalysis.h"
#include "AnalysisAdaptor.h"


class RefractiveIndex : public ofBaseApp
{
public:
    
    // ofx
    void setup();
    void update();
    void draw();
    
    // refindx
    void setup_camera();
    
    // ofx
    void keyPressed  (int key);
    void keyReleased(int key){;}
    void mouseMoved(int x, int y ){;}
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h){;}  
    
protected:
    
    void eventsIn(guiCallbackData & data);
    void grabBackgroundEvent(guiCallbackData & data);
    
    // gui
    ofxControlPanel     _gui;   
    
    // acquisition
    ofVideoGrabber      _vidGrabber;
    vector<string>      videoSourceList;
    int                 _vid_w, _vid_h, _vid_id;
    bool                _vid_stream_open;
    bool                _vid_toggle_on;
    
    AbstractAnalysis*   _currentAnalysis;  
    AnalysisAdaptor*    _analysisAdapator;

};