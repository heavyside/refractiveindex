#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //switch for controlling all analysis etc V important variable
    menuState=0;
    
    camWidth=640;
    camHeight=480;
    ofSetFrameRate(30);
    record =false;
    
    
    
    //its easier to initialise the camera with default settings than mess around with bad access errors when you try and draw it;(
    setupCamera(camWidth, camHeight,2,30,true);
    
    font.loadFont("MONACO.TTF", 10);
    
    showGui=true;
    //set initial report
    camStatus="camera not setup";
    
    masterAnalysis.setupAnalysis(camWidth, camHeight, 200, 1);//, vidGrabber);
    masterAnalysis.setupSaver(camWidth, camHeight, whichCodec);
    
    //get list of codecs from movie object in analysis class
    vector<string> returnedCodecNames=masterAnalysis.saver.returnCodecNames();
    
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
    gui.addTextInput("display message", "worship the screen mortals!", 250 );
    
    //GET THE INPUT NAMES FROM THE QT VIDEO GRABBER
    vidGrabber.listDevices();
    gui.addToggle("more cam settings", "SETTINGS", 0);
    
    //A NEW PANEL
    gui.addPanel("analysis management", 4, false);
    gui.setWhichPanel(1);
    
    //will handle which analysis we are doing this time
    int AbritratryNumberOfAnlaysis=2;
    vector<string>analysisNames;
    for(int i=0;i<AbritratryNumberOfAnlaysis;i++){
        analysisNames.push_back(ofToString(i));
    }
    gui.addTextDropDown("analysis type", "ANALYSIS_TYPE", 130, analysisNames);
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
        masterAnalysis.setupSaver(camWidth, camHeight,whichCodec);
        masterAnalysis.setupAnalysis(camWidth, camHeight, 100, analysisChooser);//, vidGrabber);
        
        //now we are setup lets analyse
        menuState=2;
        
    }
    //conduct analysis
    if(menuState==2){
       // if (vidGrabber.isFrameNew()) {
        
        //if we aren't finish, analyse
    if(!masterAnalysis.analysed){
          masterAnalysis.runAnalysis(vidGrabber.getPixels());
        }
        //if we are, save out the movie and load it into a video player
        else{
           masterAnalysis.saveOut();
           masterAnalysis.setupMovie();
            menuState=3;
        }
       // }
    }
     //display analysis
    if(menuState==3){
        masterAnalysis.updatePlayer();
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
        
        vidGrabber.draw(0, 0);
    }
    //draw analysis
    if(menuState==2){
        //  vidGrabber.draw(0, 0);
        if(!masterAnalysis.analysed){
    //    cout<<"dis playing process \n";
            masterAnalysis.displayProcess();
        }
    }
    //menustate draw results of analysis
    if(menuState==3){
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
   
    //use x as gui toggle
    if(key=='x'){
        showGui=!showGui;
    }
    else{
        
        if(showGui){
        bool control_panel_ate_key = gui.keyPressed( key );
        }
        else{
            
            if(key=='q'){
               masterAnalysis.lerpZ+=200;
            }
            if(key=='a'){
                masterAnalysis.lerpZ-=200;
            }
            
            
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
            if( data.getType(k) == CB_VALUE_INT ){
                analysisChooser=1+data.getInt(k);
                cout<<data.getInt(k)<<" lets run tHIS analysis\n";    
                
            }
            if( data.getType(k) == CB_VALUE_FLOAT ){
                analysisChooser=1+data.getFloat(k);
                cout<<data.getFloat(k)<<" lets run tHIS analysis\n";   
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

