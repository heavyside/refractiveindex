/*
 - copyright (c) 2011 Copenhagen Institute of Interaction Design (CIID)
 - all rights reserved.
 
 + redistribution and use in source and binary forms, with or without
 + modification, are permitted provided that the following conditions
 + are met:
 +  > redistributions of source code must retain the above copyright
 +    notice, this list of conditions and the following disclaimer.
 +  > redistributions in binary form must reproduce the above copyright
 +    notice, this list of conditions and the following disclaimer in
 +    the documentation and/or other materials provided with the
 +    distribution.
 
 + THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 + "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 + LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 + FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 + COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 + INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 + BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 + OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 + AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 + OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 + OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 + SUCH DAMAGE.
 
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ~ author: dviid
 ~ contact: dviid@labs.ciid.dk 
 */

#include "IResponseAnalysis.h"
#include "ofMain.h"

#include "Poco/Timer.h"
#include "Poco/Thread.h"

using Poco::Timer;
using Poco::TimerCallback;
using Poco::Thread;

#define STATE_FADE              0
#define STATE_SAVE              1
#define STATE_TRANSITION_SAVE   2
#define STATE_TRANSITION_FADE   3
#define STATE_ANALYSIS          4


void IResponseAnalysis::setup(int camWidth, int camHeight)
{
    //AbstractAnalysis::setup(camWidth, camHeight);    
    //_lastTime = ofGetElapsedTimeMillis();   
        
    if(!ofDirectory::doesDirectoryExist(string(ANALYSIS_PATH) + _name, false))
        ofDirectory::createDirectory(string(ANALYSIS_PATH) + _name, false);
        
    string xml_file = string(ANALYSIS_PATH) + _name + string("/confing.xml"); 
    cout << xml_file << endl; 
}


void IResponseAnalysis::synthesize()
{
    Timer* save_timer;// = new Timer(0, 1000);
    Timer* fade_timer;// = new Timer(0, 100);
    
    TimerCallback<IResponseAnalysis> save_callback(*this, &IResponseAnalysis::save_cb);  //
    TimerCallback<IResponseAnalysis> fade_callback(*this, &IResponseAnalysis::fade_cb);
    
    for(int i = 0; i < 5; i++) {
        
        /****** START callback for the fading between writing to screen and saving to disk *****/
        _save_cnt = 0;    
        _state = STATE_SAVE;
        save_timer = new Timer(0, 1000); //how often the save callback is being called - void IResponseAnalysis::save_cb(Timer& timer)
        save_timer->start(save_callback);
        
            while(_state != STATE_TRANSITION_SAVE)
                Thread::sleep(5);
        
        save_timer->stop();
        delete save_timer;  // stupid poco
        /****** END callback for the fading between writing to screen and saving to disk *****/
        
        /****** START callback for the fading between writing to screen and saving to disk *****/
        _fade_cnt = 0;
        _state = STATE_FADE;
        fade_timer = new Timer(0, 100);  //how often the save callback is being called - void IResponseAnalysis::fade_cb(Timer& timer)
        fade_timer->start(fade_callback);
    
            while(_state != STATE_TRANSITION_FADE)
                Thread::sleep(5);
        
        fade_timer->stop();
        delete fade_timer; // stupid poco
        /****** END callback for the fading between writing to screen and saving to disk *****/
        
    }
    
    _state = STATE_ANALYSIS;
    
    // do analysis here
    while(_state != STATE_STOP)
        Thread::sleep(100);

}

void IResponseAnalysis::gui_attach(ofxControlPanel* gui)
{
    gui->addToggle("GO", "GO", 0);
    gui->addButtonSlider("animation time limit", "ANIMATION_TIME_LIMIT", 10, 1, 3000, TRUE);
}

void IResponseAnalysis::gui_detach()
{
    
}

void IResponseAnalysis::draw()
{
    if(_state == STATE_SAVE || _state == STATE_TRANSITION_SAVE) {
        
        ofSetColor(0, 0, 0);               
        ofRect(0, 0, ofGetWidth(), ofGetHeight()); 
        
        
    } else if(_state == STATE_FADE || _state == STATE_TRANSITION_FADE) {
        
        int c = 255 / (22 - _fade_cnt);  
        cout << "draw() _fade_cnt:" << _fade_cnt << "\n";
        
        ofSetColor(c, c, c);
        ofRect(0, 0, ofGetWidth(), ofGetHeight()); 
        
    } else if(_state == STATE_ANALYSIS) {
        ofSetColor(0, 255, 0);  // green is 'done' - or analysizing 

        //for the analysis - do we need a seperate call back? is that the idea?
        
        ofRect(0, 0, ofGetWidth(), ofGetHeight());         
    }    
}

void IResponseAnalysis::save_cb(Timer& timer)
{
    //cout << "IResponseAnalysis::saving...\n";
    _save_cnt++;
    
    if(_save_cnt >= 5) { 
        //cout << "IResponseAnalysis::saving... _save_cnt\n";
    
        // trying to get an ofPixels frame here - how to get the RefractiveIndex.cpp ofPixels into this function? 
        // we used to to this by passing the ofPixels into this class - but now i think we need to have a separate camera grabber for each of these classes?  is that right? 
    
        //pixels = _vidGrabber.grabFrame();
        // ... becomes ...
        //_vidGrabberIRseponseAnalysis.grabFrame();
        // ??
        
        _state = STATE_TRANSITION_SAVE;  //
    }
}

void IResponseAnalysis::fade_cb(Timer& timer)
{
    cout << "fade_cb() _fade_cnt:" << _fade_cnt << "\n";
  
    if(_fade_cnt >= 10){ //2 secs fade counter
        _state = STATE_TRANSITION_FADE;
        
    }
    //cout << "IResponseAnalysis::fading...\n";
    _fade_cnt++;
}

