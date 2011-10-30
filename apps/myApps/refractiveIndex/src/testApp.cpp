#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    //changed for version control test
    camHeight=48023434;
    
    //switch for controlling all analysis etc V important variable
    menuState=0;
    //some change
    camWidth=640;
    camHeight=480;
    ofSetFrameRate(30);
    ofSetVerticalSync(TRUE);
    
    //its easier to initialise the camera with default settings than mess around with bad access errors when you try and draw it;(
    setupCamera(camWidth, camHeight,2,30,true);
    
    font.loadFont("MONACO.TTF", 10);
    
    showGui=true;
    //set initial report
    camStatus="camera not setup";
        
    //get list of codecs from movie object in analysis class
    vector<string> returnedCodecNames=masterAnalysis.movieFromCamera.returnCodecNames();
    
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
    analysisNames.push_back("M_CODE");
    analysisNames.push_back("CAM_FRAMERATE");
    analysisNames.push_back("CAM_NOISE");
    analysisNames.push_back("COLOR_SINGLE");
    analysisNames.push_back("PHYS_TEST");
    analysisNames.push_back("COLOR_MULTI");
    analysisNames.push_back("DIFF_NOISE");
    
    gui.addTextDropDown("analysis type", "ANALYSIS_TYPE", 130, analysisNames);
    gui.setWhichColumn(2);
    gui.addButtonSlider("scan line width", "SCAN_LINE_WIDTH", 10, 1, 100, TRUE);
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(1);
        
    vector<string> names=vidGrabber.returnDeviceNames();
    cout<<names.size()<<" number of inputs found\n";
    
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
    
}

//--------------------------------------------------------------
void testApp::update(){  
    ofBackground(0, 0, 0);
    
    vidGrabber.grabFrame();
   
    //neutral, do nothing
    if(menuState==0){

    }
    
    //setup analysis
    if(menuState==1){
        
        cout<<analysisChooser<<" menu state is 1, setting up saver and analyis\n";
        //masterAnalysis.setupSaver(camWidth, camHeight, whichCodec);  //Tom - I've removed this as it's currently redundant with the setupAnalysis below  -JA
 
        masterAnalysis.setupAnalysis(camWidth, camHeight, 100, analysisChooser);//, vidGrabber);
        
        //now we are setup lets analyse
        menuState=2;
    }
    
    //play the synthesized stuff to screen 
    if(menuState==2){
  
    }
    

    //run analysis on the movie we've recorded during 'synthDraw'
    if(menuState==3){
        // after 
        
        if(!masterAnalysis.analysed){
            masterAnalysis.analyseInput(vidGrabber.getPixels());
        } else {
            menuState=4;
        }
    }
    
    //display analysis
    if(menuState==4){
        masterAnalysis.setupMovie();
        masterAnalysis.updatePlayer();  //playing the recorded movie file
    }
    
    gui.update();
}


//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255);
    
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
            // cout<<"dis playing process \n";
            masterAnalysis.synthDrawCamRecord(vidGrabber.getPixels());
        } else {
            menuState==3;  
        } 
        
    }
    
    //menustate  draw results of analysis
    if(menuState==3){
    
    }
    

    if(menuState==4){
        
        masterAnalysis.displayResult();
        
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
    else{
    
        if(showGui){
            bool control_panel_ate_key = gui.keyPressed( key );
        }
        
        else{
            
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
    
    //SCAN LINE WIDTH 
    //
    //  testing github
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
    
    
    //CAM WIDTH
    if( thisName == "camera width" ){
        cout<<"triggered \n";
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                printf("%i float  value = %f \n", k, data.getFloat(k));
                camWidth=data.getFloat(k);
                cout<<camWidth<<" camWidth is now this\n";
            }
            else if( data.getType(k) == CB_VALUE_INT ){
                printf("%i int    value = %i \n", k, data.getInt(k));
                camWidth=data.getInt(k);
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
    
    //LIST OF CODECS
    
    if( data.getDisplayName() == "codecs" ){
        cout<<"input selected\n";
        for(int k = 0; k < data.getNumValues(); k++){
            if( data.getType(k) == CB_VALUE_FLOAT ){
                printf("%i float  value = %f \n", k, data.getFloat(k));
                
            }
            else if( data.getType(k) == CB_VALUE_INT ){
                printf("%i int    value = %i \n", k, data.getInt(k));
                //  camInput=data.getInt(k);
                cout<<camInput<<" camInput \n";
            }
            else if( data.getType(k) == CB_VALUE_STRING ){
                printf("%i string value = %s \n", k, data.getString(k).c_str());
                codecName=data.getString(k).c_str();
                whichCodec=k;
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

