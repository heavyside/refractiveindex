/*
 ~ author: dviid
 ~ contact: dviid@labs.ciid.dk 
 */

#include "ShadowScapesAnalysis.h"
#include "ofMain.h"

#include "Poco/Timer.h"
#include "Poco/Thread.h"

using Poco::Timer;
using Poco::TimerCallback;
using Poco::Thread;

#define STATE_SCAN      0
#define STATE_ANALYSIS  1

void ShadowScapesAnalysis::setup(int camWidth, int camHeight)
{
    //AbstractAnalysis::setup(camWidth, camHeight);    
    //_lastTime = ofGetElapsedTimeMillis();  
    
    _speed = 300;
}

void ShadowScapesAnalysis::synthesize()
{
    int w;
    if(_dir == H) w = ofGetWidth();
    else if(_dir == V) w = ofGetHeight();
    _step = ((w / _speed) * 1000) / 50;
    _line = 0;
    Timer scan_timer(0, 50);
    
    TimerCallback<ShadowScapesAnalysis> strobe_callback(*this, &ShadowScapesAnalysis::scan_cb);
    
    _state = STATE_SCAN;
    
    scan_timer.start(strobe_callback);
    
    while(_state != STATE_ANALYSIS)
        Thread::sleep(5);
    
    scan_timer.stop();
    // do analysis here
    // go back to the files i've saved and do the math here -     

    while(_state != STATE_STOP)
        Thread::sleep(100);
}

void ShadowScapesAnalysis::gui_attach(ofxControlPanel* gui)
{
    gui->addToggle("GO", "GO", 0);
    gui->addButtonSlider("animation time limit", "ANIMATION_TIME_LIMIT", 10, 1, 3000, TRUE);    
}

void ShadowScapesAnalysis::gui_detach()
{
    
}

// the animation draw - and the output draw 
void ShadowScapesAnalysis::draw()
{    
    static int _pos;
    
    if(_state == STATE_ANALYSIS) {
        ofSetColor(0, 200, 0);               
        ofRect(0, 0, ofGetWidth(), ofGetHeight());         
        return;
    }
    
    if(_state == STATE_SCAN) {
        if(_pos != _line) {
            //take snap??
            
            // send image back to 
            _pos = _line;
        }
        ofSetColor(255, 255,  255); 
        
        if(_dir == H) ofRect(_pos, 0, 50, ofGetHeight());
        else if(_dir == V) ofRect(0, _pos, ofGetWidth(), 50);                    
    }
}


void ShadowScapesAnalysis::scan_cb(Timer& timer)
{
    cout << "ShadowScapesAnalysis::scan_cb\n";
    
    _line += _step;
    
    if((_dir == H && _line >= ofGetWidth()) || 
       (_dir == V && _line >= ofGetHeight())) { 
        _state = STATE_ANALYSIS;
    }
}
