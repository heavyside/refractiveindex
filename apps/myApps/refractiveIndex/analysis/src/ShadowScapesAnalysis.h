/*
 ~ author: dviid
 ~ contact: dviid@labs.ciid.dk 
 */

#pragma once

#include "AbstractAnalysis.h"
#include "Poco/Timer.h"

enum shadow_type {
    H, V, D,
};

class ShadowScapesAnalysis : public AbstractAnalysis
{
public:
    ShadowScapesAnalysis(): AbstractAnalysis("SHADOWSCAPE"){;}
    //ShadowScapesAnalysis(shadow_type dir): AbstractAnalysis("SHADOWSCAPE"), _dir(dir){;}  // jamie took this out jan 3rd - typo?
    virtual ~ShadowScapesAnalysis(){;}
    
public:
    
    void setup(int camWidth, int camHeight);
    void synthesize();
    void gui_attach(ofxControlPanel* gui);
    void gui_detach();
    
    void draw();
    
    void scan_cb(Poco::Timer& timer);
    
protected:
    
    int     _line;  
    int     _speed; // pix per second
    int     _step;
    shadow_type _dir;
    
};