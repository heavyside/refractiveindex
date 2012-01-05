/*
 ~ author: dviid
 ~ contact: dviid@labs.ciid.dk 
*/

#pragma once

#include "AbstractAnalysis.h"

#include "Poco/Timer.h"

class StrobeAnalysis : public AbstractAnalysis
{
public:
    StrobeAnalysis(): AbstractAnalysis("STROBE"){;}
    virtual ~StrobeAnalysis(){;}
    
public:
    
    void setup(int camWidth, int camHeight);
    void synthesize();
    void gui_attach(ofxControlPanel* gui);
    void gui_detach();
    
    void draw();
    
    void strobe_cb(Poco::Timer& timer);
    
protected:
    
    int     _strobe_cnt;  
    bool    _darkness;
};
