#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    //changed for version control test
    camHeight=48023434;
    showGraphLine=false;
    //switch for controlling all analysis etc V important variable
    menuState=0;
    //some change
    camWidth=640;
    camHeight=480;
    ofSetFrameRate(30);
    ofSetVerticalSync(TRUE);
    //set default codec
    codecChooser=0;
    //default analysis
    analysisChooser="M_CODE";
    
    //its easier to initialise the camera with default settings than mess around with bad access errors when you try and draw it;(
    setupCamera(camWidth, camHeight,2,30,true);
    
    font.loadFont("MONACO.TTF", 10);
    
    showGui=true;
    //set initial report
    camStatus="camera not setup";
        
    
    //talk to me about your troubles openframeworks
    ofSetLogLevel(OF_LOG_VERBOSE);  
    
    ////////////GUI SETUP STUFF////////////////
    ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 60, 200));
	ofxControlPanel::setTextColor(simpleColor(0, 250, 0, 255));
    
	gui.loadFont("MONACO.TTF", 8);		
	gui.setup("refractive index", 0, 0, ofGetWidth(), ofGetHeight());
	
    //FIRST PANEL HOLDS CAMERA CONTROLS
    gui.addPanel("camera control", 4, false);
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    gui.addToggle("set up camera input", "CAM_IS_GO", 0);
    gui.addButtonSlider("camera width", "CAM_WIDTH", camWidth, 1.0, 1920, true);
	gui.addButtonSlider("camera height", "CAM_HEIGHT", camHeight, 1.0, 1080, true);
    gui.addButtonSlider("desired frame rate", "FRAME_RATE", 1., 1.0, 30, true);
	gui.setWhichColumn(1);
    gui.addTextInput("display message", "input here for the text input field!", 250 );
    
    //GET THE INPUT NAMES FROM THE QT VIDEO GRABBER
    vidGrabber.listDevices();
    gui.addToggle("more cam settings", "SETTINGS", 0);
    
    //A NEW PANEL
    gui.addPanel("analysis management", 4, false);
    gui.setWhichPanel(1);
    
    //will handle which analysis we are doing this time
 
    vector<string>analysisNames;
    
    analysisNames.push_back("H_SHADOWSCAPES");
    analysisNames.push_back("V_SHADOWSCAPES");
    analysisNames.push_back("D_SHADOWSCAPES");
    analysisNames.push_back("RELAXRATE");
    analysisNames.push_back("I_RESPONSE");
    analysisNames.push_back("SHAPE_SHADING");
    analysisNames.push_back("M_CODE");
    analysisNames.push_back("CAM_FRAMERATE");
    analysisNames.push_back("CAM_NOISE");
    analysisNames.push_back("COLOR_SINGLE");
    analysisNames.push_back("PHYS_TEST");
    analysisNames.push_back("COLOR_MULTI");
    analysisNames.push_back("DIFF_NOISE");
    
    gui.addTextDropDown("analysis type", "ANALYSIS_TYPE", 130, analysisNames);
    gui.setWhichColumn(2);
    /*  float maxResultA; 
     float maxTimeA; 
     float divisionsA; 
     bool showGraphA;*/
    
    gui.addButtonSlider("scan line width", "SCAN_LINE_WIDTH", 10, 1, 100, TRUE);
    gui.addButtonSlider("scan line speed", "SCAN_LINE_SPEED", 10, 1, 100, TRUE);
    gui.addButtonSlider("max white level to ramp to", "GRAPH_MAX_RESULT", 10, 1, 255, TRUE);
    gui.addButtonSlider("num of frames to last for", "GRAPH_MAX_TIME", 10, 1, 255, TRUE);
    gui.addButtonSlider("num of complete cycles", "GRAPH_NUM_DIVISIONS", 10, 1, 10, TRUE);
    gui.addToggle("show graph outlines", "SHOW_GRAPH_OUTLINE", 0);
    gui.addButtonSlider("animation time limit", "ANIMATION_TIME_LIMIT", 10, 1, 3000, TRUE);
    gui.addButtonSlider("morse speed", "MORSE_SPEED", 10, 1, 25, TRUE);
    //gui.addTextInput("morse output", "input morse here", 250 );
    //nasty hack for getting text back
    tl=gui.addTextInput("morse output", "USE_UNDERSCORES_AND_CAPS", 250 );
    gui.addButtonSlider("red level", "RED_LEVEL", 10, 1, 255, TRUE);
    gui.addButtonSlider("green level", "GREEN_LEVEL", 10, 1, 255, TRUE);
    gui.addButtonSlider("blue level", "BLUE_LEVEL", 10, 1, 255, TRUE);
    //only 4 options for relax rate so far.
    vector<string>graphNames;
    graphNames.push_back("LINEAR");
    graphNames.push_back("EXPONENTIAL");
    graphNames.push_back("SQUARE_WAVE");
    graphNames.push_back("QUADRATIC");
    
    gui.addTextDropDown("graph type", "GRAPH_TYPE", 130, graphNames);
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(1);
        
    vector<string> names=vidGrabber.returnDeviceNames();
    cout<<names.size()<<" number of inputs found\n";
    masterAnalysis.setupAnalysis(camWidth, camHeight, 100, analysisChooser, codecChooser);//, vidGrabber);
    
    masterAnalysis.setGUIDefaults();
    
    //get list of codecs from movie object in analysis class
    returnedCodecNames=masterAnalysis.movieFromCamera.returnCodecNames();
    
    //CURRENTLY UNUSED
    cout<<names[names.size()-1]<<" names at 2\n";
    gui.addTextDropDown("inputs", "INPUTS", 130, names);
    gui.setWhichColumn(2);
    gui.addTextDropDown("codecs", "CODECS", 130, returnedCodecNames);
    
    //dont forget to actually set this stuff in action!
    gui.setupEvents();
	gui.enableEvents();
    //  -- this gives you back an ofEvent for all events in this control panel object
	ofAddListener(gui.guiEvent, this, &testApp::eventsIn);
    
    ////////////END OF GUI SETUP STUFF////////////////
    masterAnalysis.setGUIDefaults();
   
    
}

//--------------------------------------------------------------
void testApp::update(){  
    ofBackground(0, 0, 0);
   
    string str = "framerate is "; 						
    str += ofToString(ofGetFrameRate(), 2)+"fps"; 
    ofSetWindowTitle(str);
    //set the window title to "framerate"
    
    vidGrabber.grabFrame();  // go get frame from OS
   
//  vidGrabber.update();  // go get frame from OS
    
    
    //neutral, do nothing
    if(menuState==0){

    }
    
    //setup analysis
    if(menuState==1){
        
        cout<<analysisChooser<<" menu state is 1, setting up saver and analyis\n";
       
        masterAnalysis.morseMessage= tl->getValueText();
        masterAnalysis.setupAnalysis(camWidth, camHeight, 100, analysisChooser, codecChooser);//, vidGrabber);
        
        //now we are setup lets analyse
        menuState=2;
    }
    
    //play the synthesized stuff to screen 
    if(menuState==2){
       
    }
    
    //run analysis on the movie we've recorded during 'synthDraw'
    if(menuState==3){
        // after 
    }
    
    //display analysis
    if(menuState==4){
        //masterAnalysis.setupMovie();
        //masterAnalysis.updatePlayer();  //playing the recorded movie file
    }
    
    gui.update();
}


//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(0);
    
    //if no analysis, draw grabber
    if(menuState==0){
        vidGrabber.draw(0, 0);
    }
    
    //continue to draw grabber in setup phase
    if(menuState==1){
        //vidGrabber.draw(0, 0);
    }
    
    //drawing synthesized impulses, etc., to the screen
    if(menuState==2){
        
        //vidGrabber.draw(0, 0);
        if(!masterAnalysis.synthesisComplete){
            //cout<<masterAnalysis.synthesisComplete<<"masterAnalysis.synthesisComplete \n";
            //cout<<"in draw loop menuState 2 \n";
            
            if (vidGrabber.isFrameNew())
            {   
                camPixels = vidGrabber.getPixelsRef();
            } 
                masterAnalysis.synthDrawCamRecord(camPixels);            
            
        } else {
            menuState = 3;
        }
    }
    
    //menustate  draw results of analysis
    if(menuState==3){
        //cout<<" delete [] camPixels; \n";
       
        
        //cout<<"in draw loop menuState 3 \n";
    }
    

    if(menuState==4){
        //masterAnalysis.displayResult();
        
    }
    
    if(showGui){
        gui.draw();
        font.drawString(camStatus,50, ofGetHeight()-50);
    }
}


//---------------  CALLED FROM INSIDE EVENT FUNCTION WHEN CAMERA SETTINGS HAVE BEEN SELECTED ---------------------------------------
void testApp::setupCamera(int w, int h, int whichSource,int desiredFrameRate, bool firstSetup){
    cout<<whichSource<<" this source\n";
    
    vidGrabber.close();
    
    camWidth = w;
    camHeight =h;
    
    vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID(whichSource);
    vidGrabber.initGrabber(camWidth,camHeight);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
   
    //use g as gui toggle
    if(key=='g'){
        showGui=!showGui;
    }
    
    else {
    
        if(showGui){
            bool control_panel_ate_key = gui.keyPressed( key );
        }
        else {
        }
    }
}
//----------------------NOT CURRENTLY USED----------------------------------------
void testApp::grabBackgroundEvent(guiCallbackData & data){
	
	//we use the event callback to capture the background - we then set the toggle value back to its previous value
	if( data.isElement( "GRAB_BACKGROUND" ) && data.getInt(0) == 1 ){
        //	bgExample.captureBackground();
		gui.setValueB("GRAB_BACKGROUND", false);
	}
}


//this captures all our control panel events - unless its setup differently in testApp::setup
//--------------------------------------------------------------
void testApp::eventsIn(guiCallbackData & data){
    
	//lets send all events to our logger
	if( !data.isElement( "events logger" ) ){
		string logStr = data.getXmlName();
		
		for(int k = 0; k < data.getNumValues(); k++){
			logStr += " - " + data.getString(k);
		}
		
		logger.log(OF_LOG_NOTICE, "event - %s", logStr.c_str());
	}
	
	// print to terminal if you want to 
	//this code prints out the name of the events coming in and all the variables passed
    
    //SETUP CAMERA
	printf("testApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
    if( data.getDisplayName() == "set up camera input" ){
        //vidGrabber.close();
        setupCamera(camWidth,camHeight,camInput,desFrameRate,false);

        //menuState=1;
        cout<<"cam is setup\n"; 
    }
    
    string thisName=data.getDisplayName();
    cout<<thisName<<" this Name\n";
    
    //animation time limit
    if( thisName == "animation time limit" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                masterAnalysis.animationTimeLimit=data.getFloat(k);
                cout<<masterAnalysis.animationTimeLimit<<"masterAnalysis.animationTimeLimit \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    if( thisName == "scan line width" ){
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.scanLineWidth=data.getFloat(k);
                cout<<masterAnalysis.scanLineWidth<<"masterAnalysis.scanLineWidth \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    if( thisName == "scan line speed" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                

                masterAnalysis.scanLineSpeed=data.getFloat(k);
                cout<<masterAnalysis.scanLineSpeed<<"masterAnalysis.scanLineSpeed \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    if( thisName == "max white level to ramp to" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                

                masterAnalysis.maxResultA=data.getFloat(k);
                cout<<masterAnalysis.maxResultA<<"masterAnalysis.maxResultA \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    if( thisName == "num of frames to last for" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){

                masterAnalysis.maxTimeA=data.getFloat(k);
                cout<<masterAnalysis.maxTimeA<<"masterAnalysis.maxTimeA \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    if( thisName == "num of complete cycles" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.divisionsA=(2*(int)data.getFloat(k));
                cout<<"masterAnalysis.divisionsA = "<<masterAnalysis.divisionsA<<"  \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    if( thisName == "show graph outline" ){
        showGraphLine=!showGraphLine;
        masterAnalysis.showGraphA=showGraphLine;
        
    }
    //Tom S 14 Nov 19:10somehow this function appears to have been repeated so have commented out
   /* if( thisName == "graph max time" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){

                masterAnalysis.maxTimeA=data.getFloat(k);
                cout<<masterAnalysis.maxTimeA<<"masterAnalysis.maxTimeA \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }*/
    
    if( thisName == "morse speed" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
               
                masterAnalysis.speed=data.getFloat(k);
                cout<<masterAnalysis.speed<<"masterAnalysis.speed \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    if( thisName == "red level" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.red=data.getFloat(k);
                cout<<masterAnalysis.red<<" masterAnalysis.red \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    if( thisName == "green level" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.green=data.getFloat(k);
                cout<<masterAnalysis.green<<" masterAnalysis.green \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    if( thisName == "red level" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                
                masterAnalysis.blue=data.getFloat(k);
                cout<<masterAnalysis.blue<<" masterAnalysis.blue \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
        
    }
    //more cam settings", "SETTINGS"
    if( data.getDisplayName() == "more cam settings" ){
        vidGrabber.videoSettings();
        
    }
    
    //START ANALYSIS
   // string isThisAnAnlysisButton = data.getDisplayName().substr(0,12);
    if( data.getDisplayName()== "analysis type" ){
       
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_STRING ){
                analysisChooser=data.getString(k);
                cout<<data.getString(k)<<" lets run tHIS analysis\n";    
            }
        }
        showGui=false;
        menuState=1;
    }
    
    //WHAT KIND OF GRAPH
    if( data.getDisplayName()== "graph type" ){
        
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_STRING ){
                
                //copy string straight into class
                masterAnalysis.whichGraph=data.getString(k);
                cout<<data.getString(k)<<" lets run tHIS analysis\n";    
            }
        }
       
    }
    
    //CAM HEIGHT
    if( data.getDisplayName() == "camera height" ){
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                printf("%i float  value = %f \n", k, data.getFloat(k));
                camHeight=data.getFloat(k);
            }
            else if( data.getType(k) == CB_VALUE_INT ){
                printf("%i int    value = %i \n", k, data.getInt(k));
                camHeight=data.getInt(k);
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
            }
        }
    }
    
    //FRAME RATE ISNT WORKING - I THINK I HACKED THE LIBRARY TODO SET THIS RIGHT
    if( data.getDisplayName() == "desired frame rate" ){
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                printf("%i float  value = %f \n", k, data.getFloat(k));
                desFrameRate=data.getFloat(k);
            }
            else if( data.getType(k) == CB_VALUE_INT ){
                printf("%i int    value = %i \n", k, data.getInt(k));
                desFrameRate=data.getInt(k);
            }
        }
    }
    
    //LIST OF CAM SOURCES
    if( data.getDisplayName() == "inputs" ){
        cout<<"input selected\n";
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                printf("%i float  value = %f \n", k, data.getFloat(k));
            }
            else if( data.getType(k) == CB_VALUE_INT ){
                printf("%i int    value = %i \n", k, data.getInt(k));
                camInput=data.getInt(k);
                cout<<camInput<<" camInput \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
                camInputName=data.getString(k).c_str();
            }
        }
    }
    
    // TOM - doesn't this refer to the morse text INPUT on the GUI? 
    if( data.getDisplayName() == "morse output" ){
        cout<<"getting morse message\r";
    }
    
    //LIST OF CODECS
    
    if( data.getDisplayName() == "codecs" ){
        cout<<"input selected\n";
        for(int k = 0; k < data.getNumValues(); k++){
      
            if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
                codecName=data.getString(k).c_str();
                cout<<codecName;
                //compare the string returned from the gui to get the number so we can pass this into the analysis setup
                for(int i=0;i<returnedCodecNames.size();i++){
                    
                    if(returnedCodecNames[i]==codecName){
                        codecChooser=i;
                    }
                }
                
                
            }
        }
    }
    
    //UPDATE THE REPORT STRING FOR CURRENT SETTINGS
    camStatus="CAMERA STATUS:: width :"+ofToString(camWidth)+" height :"+ofToString(camHeight)+" frame rate :"+ofToString(desFrameRate)+"\nsource :"+camInputName+"\ncodec: "+codecName;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if( key =='f')
    {
       ofToggleFullscreen(); 
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
  //  mouseX=x;
   // mouseY=y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    gui.mousePressed(x, y, button); 
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    gui.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}