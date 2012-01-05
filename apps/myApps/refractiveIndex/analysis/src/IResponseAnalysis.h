/*
~ author: dviid
~ contact: dviid@labs.ciid.dk 
*/

#pragma once

#include "AbstractAnalysis.h"

#include "Poco/Timer.h"

class IResponseAnalysis : public AbstractAnalysis
{
public:
    IResponseAnalysis(): AbstractAnalysis("I_RESPONSE"){;}
    virtual ~IResponseAnalysis(){;}
    
public:
    
    void setup(int camWidth, int camHeight);
    void synthesize();
    void gui_attach(ofxControlPanel* gui);
    void gui_detach();
    
    void draw();
    
    void save_cb(Poco::Timer& timer);
    void fade_cb(Poco::Timer& timer);
    
protected:
        
    float   _save_cnt, _fade_cnt, _fade_cnt_max;
    
};
