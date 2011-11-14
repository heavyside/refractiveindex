#include "analysis.h"

//there is a problem with re-using the same filename so this is a hacky solution
//what's the solution here?  need to clear the folder or the video seems to always come out black? 
int movieNameCounter = 0;

////////////////////////---------/////////////////////////////////////
//SETUP THE CLASS
//Composed of things this class will always need, i.e.: a way of synthesizing the outputs, recording images
//from the camera, analysing the recorded data and outputting resulting info to the screen.

////////////////////////---------/////////////////////////////////////
void analysis::setupAnalysis(int camW, int camH, int analasisTimePass, string whichAnalysisPass, int whichCodec){//, ofVideoGrabber &grabber){
    //i included an argument which is the pointer to the grabber in case this is better than passing in pixel array? not currently used

    whichAnalysis = whichAnalysisPass;
    analysisTime = analasisTimePass;
    
    cout<<whichAnalysis<<" WHICH ANALYSIS AT SETUP\n";
    cout<<"chosen codec number "<<whichCodec<<"\r";
    camHeight=camH;
    camWidth=camW;
    check=0;
    counter=0;
    frameCounter = 0;
    synthesisComplete=FALSE;
   
    // dataPathName = "/Users/jamieallen/Projects/newcastle/projects/RefractiveIndexLaptop/openframeworks/refractiveindex/apps/myApps/refractiveIndex/bin/data/MEDIA/";
    // dataPathName=ofToDataPath("")+"MEDIA/";
    // ofSetDataPathRoot(dataPathName);
    // probably good in future to have one of these for each analysis - and then have the folder name increment for each 'run' of the analysis at a given site
    // TOM-> JAMIE dod you mean have a different folder? if so why don't we just use the analysis names
    // as the folder names we could also set the site name in the gui if you like and use this? we could have a folder for each site with subfolders for each analysis
    
    //this is the short cut to get the data path (ie location of "data folder") ofToDataPath("") so ofToDataPath("")+"MEDIA" is prob what we want here

    
    //Setups for the specific analyses as needed...    
    if (whichAnalysis=="H_SHADOWSCAPES") {
        scanLinePosition= 0; 
        //scanLineWidth = 50;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
        //because this is called after the gui sets this - that's why ;) 
    
        // scanLineSpeed is now set in gui for all shadowscapes
    }

    if (whichAnalysis=="V_SHADOWSCAPES") {
        scanLinePosition=0; 
        //scanLineWidth = 25;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
        //scanLineSpeed = 10;
    } 

    if (whichAnalysis=="D_SHADOWSCAPES") {
        scanLinePosition = 0; 
       // scanLineWidth = 15;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
        //scanLineSpeed = 10;
    } 
    
    if (whichAnalysis=="RELAXRATE") {
        //SETUP VIDEOSAVER
        //this function initalises on the variables for the graphs
        setupGraphs();
    } 
    
    if (whichAnalysis=="I_RESPONSE") {
        
        lastTime = ofGetElapsedTimeMillis();
        
        //amimationTimeLimit is now set in GUI
        //animationTimeLimit = 3000;  //milliseconds
        
        timeDiff = 0;
        counter = 0;
        frameCounter = 0;
        level = 0;
        
        //TODO: Set fadeTime in GUI
        fadeTime = 5000;
        
        //TODO: Set fadeTime in GUI
        everyNthFrameCaptureImage = 2; //the lower this number the more camera images we get per white level shown
        numberOfGreyLevels = 10;   //the number of grey levels we want to look at
        currentGreyLevel = numberOfGreyLevels; 
    }
    
    if (whichAnalysis=="SHAPE_SHADING") {
        counter=0;
        ofSetCircleResolution(100);
        ofEnableSmoothing();
    } 

    
    if (whichAnalysis=="CAM_NOISE") {
        counter = 0;
        frameCounter = 0;
        localFrameCounter = 0;
        framesPerGreyValue = 20;
        localFrameCounter = 0;
        newFrame = true;
        numberOfGreyLevels = 5.0;  //this number sets the number of grey levels that are shown  for each noise measurement
        k=1;// need to get my frames
    } 
    
    if (whichAnalysis=="COLOR_SINGLE") {
        frameCounter = 0;
        framesPerColourValue = 5;
        localFrameCounter = 0;
        gotAllLocalFrames1 = false;  //should generalise these based on the # of colors we want to use
        gotAllLocalFrames2 = false;
        gotAllLocalFrames3 = false;
        counterMax = 300.0;
        counter = 0;
    } 

    if (whichAnalysis=="M_CODE") {
        //im going to borrow some of these variables
        setupGraphs();
        onCounter=0;
        offCounter=0;
        pauseBetween=0;
        //load morse code from text file 
        loadMorse();
        morseComplete = FALSE;
        morseMessage=translateToMorse(morseMessage);
    }
    
    if (whichAnalysis=="PHYS_TEST") {
     
    }
        
    if (whichAnalysis=="CAM_FRAMERATE") {
        setupGraphs();
        strobeCounter = 0;
        synthesisComplete=FALSE;
        strobeToggleCounter = 0.0000001;
        currentSRate = 0;
        strobeComplete = FALSE;
    } 

    if (whichAnalysis=="COLOR_MULTI") {
    
    }
    
    if (whichAnalysis=="DIFF_NOISE") {
 
    
    }
 
    if (whichAnalysis=="COLOR_SINGLE") {
        counter=0;
        synthesisComplete=false;
        cout<<  synthesisComplete<<"color single setup\r";
        
    } 

}

////////////////////////---------/////////////////////////////////////
//SYNTH METHODS
//all methods function called "synth" are about playing initial impulses, etc., to the screen
////////////////////////---------/////////////////////////////////////

void analysis::synthUpdate(){
    
}

/*
1 analysisNames.push_back("H_SHADOWSCAPES");
2 analysisNames.push_back("V_SHADOWSCAPES");
3 analysisNames.push_back("D_SHADOWSCAPES");
4 analysisNames.push_back("RELAXRATE");
5 analysisNames.push_back("I_RESPONSE");
6 analysisNames.push_back("SHAPE_SHADING");
7 analysisNames.push_back("M_CODE");
8 analysisNames.push_back("CAM_FRAMERATE");
9 analysisNames.push_back("CAM_NOISE");
10 analysisNames.push_back("COLOR_SINGLE");
11 analysisNames.push_back("PHYS_TEST");
12 analysisNames.push_back("COLOR_MULTI");
13 analysisNames.push_back("DIFF_NOISE");
*/


void analysis::synthDrawCamRecord(ofPixels pixels){

    if(whichAnalysis=="H_SHADOWSCAPES"){ 
 
        // cout<<"if(whichAnalysis=="H_SHADOWSCAPES") \n";
        if(synthesisComplete==FALSE){    

            //draw the scanning bar
            ofSetColor(255, 255, 255);            
            ofRect(scanLinePosition, 0, scanLineWidth, ofGetWidth());
            scanLinePosition += scanLineSpeed;
        
            vectorOfPixels.push_back(pixels);
            frameCounter++;
                        
            if(scanLinePosition >= ofGetWidth()+(scanLineSpeed+scanLineWidth)) {
                for (i = 0; i < frameCounter; i++)  
                {   
                    //cout<<i<<"< i in H_SHADOWSCAPES ** frames being written to images \n";
                    ofSaveImage(vectorOfPixels[i], whichAnalysis+"_"+ofToString(100*i*scanLineSpeed/ofGetWidth())+"%_"+ofToString(i)+".jpg", OF_IMAGE_QUALITY_BEST);

                    //ofSaveImage(vectorOfPixels[i], whichAnalysis+"_"+"_"+ofToString(i)+".jpg", OF_IMAGE_QUALITY_BEST);
                }
                vectorOfPixels.clear(); //empty out the vector
                counter = 0;
                frameCounter = 0;
                scanLinePosition=0;
                synthesisComplete=TRUE; 
                cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
            }   
            
        } else { 
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
    }
    
    
    //The V_SHADOWSCAPES synth 
    
    if(whichAnalysis=="V_SHADOWSCAPES"){
                
        if(synthesisComplete==FALSE){    
      
            //draw the scanning bar
            ofSetColor(255, 255, 255);            
            ofRect(0, scanLinePosition, ofGetWidth(),scanLineWidth);
            scanLinePosition += scanLineSpeed;
            
            vectorOfPixels.push_back(pixels);
            frameCounter++;
            
            if(scanLinePosition >= ofGetHeight()+(scanLineSpeed+scanLineWidth)) {
                for (i = 0; i < frameCounter; i++)  
                {   
                    //cout<<i<<"< i in H_SHADOWSCAPES ** frames being written to images \n";
                    ofSaveImage(vectorOfPixels[i], whichAnalysis+"_"+ofToString(100*i*scanLineSpeed/ofGetHeight())+"%_"+ofToString(i)+".jpg", OF_IMAGE_QUALITY_BEST);
                }
                vectorOfPixels.clear(); //empty out the vector
                counter = 0;
                frameCounter = 0;
                scanLinePosition=0;
                synthesisComplete=TRUE; 
                cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";               
            }
            
        } else {
           cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
    }
    
    
    //The D_SHADOWSCAPES synth and record method below should load the images as an array of in-RAM pixels -
    //i.e.: let's see if we can speed up the render by holding the camera data in memory
    //something like - ofImageRecorderGrid example or tracerPix_src_v1.0
    
    if(whichAnalysis=="D_SHADOWSCAPES"){
        
        if(synthesisComplete==FALSE){    
            
            //draw the scanning bar
            ofSetColor(255, 255, 255); 
            ofBeginShape();
                ofVertex(0,0+scanLinePosition);
                ofVertex(0,-scanLineWidth+scanLinePosition);
                ofVertex(ofGetWidth(),-ofGetHeight()+scanLinePosition);
                ofVertex(ofGetWidth(),-ofGetHeight()+scanLineWidth+scanLinePosition);
            ofEndShape();
            scanLinePosition += scanLineSpeed;
            //cout<<scanLinePosition<<" <-- D_SHADOWSCAPES scanLinePosition: \n";
            
            vectorOfPixels.push_back(pixels);
            frameCounter++;
            
            if(scanLinePosition > 2*ofGetHeight()+(scanLineSpeed+scanLineWidth)) {
                //cout<<scanLinePosition<<" <-- scanLinePosition \n";
                //cout<<ofGetHeight()<<" <-- ofGetHeight() \n";
                //cout<<" ** scanLinePosition > ofGetHeight() \n";
                
                for (i = 0; i < vectorOfPixels.size(); i++)
                {
                    ofSaveImage(vectorOfPixels[i], whichAnalysis+"_"+ofToString((100*i*scanLineSpeed)/(2*ofGetHeight()))+"%_"+ofToString(i)+".jpg", OF_IMAGE_QUALITY_BEST);                
                }
                vectorOfPixels.clear(); //empty out the vector
                counter = 0;
                frameCounter = 0;
                scanLinePosition=0;
                synthesisComplete=TRUE; 
                cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";   
            }
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        
    }

    //The analysis as a whole going to require some kind of "wait state" for us to be able to keep something on the screen 
    //for long enough to get an average overall light reading?  i.e: cosmic latte?
    
    //*************  I THINK THIS IS THE MOST PROMISING WAY TO DO THE FILE SAVING - although it could be heavily RAM dependent?! **********//
    if(whichAnalysis=="RELAXRATE"){
        
        if(synthesisComplete==FALSE){    
            
            float lightLevel = 0;
            //CURVE RELAXES
            //cout<<"max result and maxtime "<<maxResultA<<" "<< maxTimeA<<" "<<divisionsA<<"\r";
            
            if(whichGraph=="LINEAR"){
                lightLevel = linear(maxResultA, maxTimeA, divisionsA, showGraphA);
            }
            
            // TODO:  TOM - could you hook up 'division' to the GUI for exponential?  and also i don't think the math is exactly right in the function - division is hardcoded for now
            if(whichGraph=="EXPONENTIAL"){
                lightLevel = exponential(maxResultA, maxTimeA, divisionsA, showGraphA);
            }
            
            if(whichGraph=="SQUARE_WAVE"){
                lightLevel = 255*squareWave(maxResultA, maxTimeA, divisionsA, showGraphA);
            }
            
            //TODO: NEED TO REMOVE THIS from functions and GUI
            /*
            if(whichGraph=="QUADRATIC"){
                quadratic(maxResultA, maxTimeA, divisionsA, showGraphA);
            }
             */
            
            //ofFill();
            ofSetColor(lightLevel);
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
            
            vectorOfPixels.push_back(pixels);            
            lightLevels.push_back(lightLevel);
            frameCounter++;
            
            cout<<frameCounter<<" <<-- frameCounter in RELAXRATE \n";

            //once we've finished synthesis and capturing all the frames into RAM, we can then write the
            //image vectors "imgs" backinto a quicktime movie...
                        
            if (finishedGraph) {
                string fileName; 
                //cout << vectorOfPixels.size()<< "<< -- vectorOfPixels.size() \n";
                for (i = 0; i < vectorOfPixels.size(); i++)
                {
                    ///cout<<lightLevels[i]<<"< lightLevels[i] in RELAXRATE \n";
                    //cout<<i<<"< i in RELAXRATE ** frame add counter \n";
                
                    //FOR SOME FUCKING REASON... the ofToString(lightLevels[i]) included below causes the files to write out of order!?  REALLY weird.
                    //fileName = whichAnalysis+"_"+whichGraph+"_"+ofToString(lightLevels[i],2)+"_"+ofToString(i)+".jpg";
                    //The below does the same thing...
                    //ofSaveImage(vectorOfPixels[i], whichAnalysis+"_"+whichGraph+"_"+ofToString(lightLevels[i])+"_"+ofToString(i)+".jpg", OF_IMAGE_QUALITY_BEST);                

                    //Might have something to do with using ofToString on a vector???
                    
                    //using the simpler one for now - JA Nov 13 
                    
                    //Tom Nov 14th 19:03:01 ;) no its to do with the way that osx reads file names - I just switched the order of fToString(lightLevels[i]) and ofToString(i)and its fine
                    fileName = whichAnalysis+"_"+whichGraph+"_"+ofToString(i)+"_"+ofToString(lightLevels[i],2)+".jpg";
                   // fileName = whichAnalysis+"_"+whichGraph+"_"+ofToString(i)+".jpg";
                    
                    ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);  
                }
            
                vectorOfPixels.clear(); //empty out the vector
                lightLevels.clear();
                frameCounter = 0;
                scanLinePosition=0;
                synthesisComplete=TRUE; 
                cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";   
            }            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
    }
    
    //The I_RESPONSE - see google docs
    if(whichAnalysis=="I_RESPONSE"){
       
           if(synthesisComplete == FALSE ){     //other tests of file readyness, etc. go here 
            /*
            //the below takes in the pixel as raw unsigned chars from the camera, 
            //stores these in a vector, until the on-screen synthesis is finished 
            //then the whole set of buffered images is written 
            //TO A SET OF INDIVIDUAL JPGS
            //(this is pretty much the last way i can think of doing this file saving bit...) 
                        
            //cout<<counter<<" <-- RELAXRATE COUNTER: \n";
            
            // white rectangle that stays on the screen for 5000 ms, then fades out over 2000 ms 
            // take 5 images over the first 5 seconds from the camera, but then none during the fade out

            // want to hold the programme here - take a few images while the screen light level 
            // is at held constant, then fade what's on the screen away (this isn't for analytical purposes... just so it looks nice 
            // then we 'hold' at these constant levels of 255, 254, 253 
             */
            
           if (currentGreyLevel > 0) {
                
                thisTime = ofGetElapsedTimeMillis(); 
                timeDiff = thisTime-lastTime; 
                //cout<<timeDiff<<"<-- timeDiff \n";
                //cout<<counter2<<"<-- counter2 \n";
           
               if (timeDiff < animationTimeLimit) {
                      
                    ofSetColor(255.0*(currentGreyLevel/numberOfGreyLevels), 255.0*(currentGreyLevel/numberOfGreyLevels), 255.0*(currentGreyLevel/numberOfGreyLevels));            
                    ofRect(0, 0, ofGetWidth(), ofGetHeight());

                    //cout<<timeDiff%everyNthFrameCaptureImage<<"<-- timeDiff%everyNthFrameCaptureImage \n";                        
                    /*
                    //this is kind of inexact as timeDiff returns semi-random numbers... what to do here instead?
                    //basically trying to get an exact number of images over the 5000 ms that the given white values is on screen
                    //e.g.: one per second is what i'm attempting
                    */  
                    counter++;
                    //cout<<counter<<"<-- counter \n";                        
                    testInt = counter/everyNthFrameCaptureImage;
                    //cout<<testInt<<"<-- counter/everyNthFrameCaptureImage \n";                        
                    
                    if (counter%everyNthFrameCaptureImage == 0)  
                    {
                        vectorOfPixels.push_back(pixels);
                        //frameCounter++;
                        //cout<<frameCounter<<" <<-- frameCounter in I_RESPONSE \n";
                    }
               } 
               
               if ((timeDiff >= animationTimeLimit) && (timeDiff <= animationTimeLimit+fadeTime) ){
                    //cout<<"<-- inside first else if \n";
                    
                    //ofSetColor((255.0*(counter2/counter2max))-(timeDiff/3000.0),(255.0*(counter2/counter2max))-(timeDiff/3000.0),(255.0*(counter2/counter2max))-(timeDiff/3000.0)); 
                    
                    level = (255.0*(currentGreyLevel/numberOfGreyLevels))-(255.0*(currentGreyLevel/numberOfGreyLevels)*(timeDiff-animationTimeLimit)/(fadeTime));
                    
                    //cout<<testFloat<<"<-- (255.0*(counter2/counter2max))-255*(timeDiff/3000.0); \n";
                   
                    ofSetColor(level);                     
                    ofRect(0, 0, ofGetWidth(), ofGetHeight());                    
                } 
               
               if (timeDiff > (animationTimeLimit+fadeTime)) {
                    //cout<<"<-- inside second else if \n";
                    ofSetColor(0, 0, 0);               
                    ofRect(0, 0, ofGetWidth(), ofGetHeight());

                    //once we've finished synthesis and capturing all the frames into RAM, we can then write the
                    //ofPixel vectors backinto image files
                    string fileName; 
                    for (i = 0; i < vectorOfPixels.size(); i++)
                    {
                        fileName = whichAnalysis+"_"+ofToString(currentGreyLevel)+"_"+ofToString(i)+".jpg";
                        ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                        //cout<<i<<"< i in I_RESPONSE ** frames being written to images \n";
                    }
                    vectorOfPixels.clear(); //empty out the vector
                    frameCounter = 0;
                    counter=0;
                    scanLinePosition=0;
                    currentGreyLevel--;
                    lastTime = ofGetElapsedTimeMillis();
                } 
               
           } else {
               synthesisComplete=TRUE; 
               cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n"; 
           }     
            
           } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
           }
    }       
    
    //skkpping this one for the moment...  This is going to be complicated - hoping for help from DAVID G
    
    //Tom S added copy of shapes from "Trimensional" iphone app. 
    if(whichAnalysis=="SHAPE_SHADING"){
        int xPos;
        int yPos;
        float circleDia=ofGetWidth()*0.33;
        float smallCircleDia=ofGetWidth()*0.3;
        if(synthesisComplete == FALSE ){ 
            if (counter<maxTimeA) {
                
                //draw top middle
                if(counter<maxTimeA*0.25){
                    xPos= ofGetWidth()*0.5;
                    yPos=0;
                    ofSetColor(155);
                    ofCircle(xPos-circleDia/2, yPos, smallCircleDia);
                    ofCircle(xPos+circleDia/2, yPos, smallCircleDia);
                    ofSetColor(255);
                    ofCircle(xPos, yPos, circleDia);
                    
                }
                
                
                //draw right
                if(counter>=maxTimeA*0.25 && counter<maxTimeA*0.5){
                    xPos= ofGetWidth();
                    yPos=ofGetHeight()*0.5;
                    ofSetColor(155);
                    ofCircle(xPos, yPos-circleDia/2, smallCircleDia);
                    ofCircle(xPos, yPos+circleDia/2, smallCircleDia);
                    ofSetColor(255);
                    ofCircle(xPos, yPos, circleDia);
                    
                    
                    
                    
                    
                }
                
                
                //draw bottom middle
                if(counter>=maxTimeA*0.5 && counter<maxTimeA*0.75){
                    xPos= ofGetWidth()*0.5;
                    yPos=ofGetHeight();
                    ofSetColor(155);
                    ofCircle(xPos-circleDia/2, yPos, smallCircleDia);
                    ofCircle(xPos+circleDia/2, yPos, smallCircleDia);
                    ofSetColor(255);
                    ofCircle(xPos, yPos, circleDia);
                    
                    
                }
                //draw left
                if(counter>=maxTimeA*0.75 && counter<maxTimeA){
                    xPos= 0;
                    yPos=ofGetHeight()*0.5;
                    ofSetColor(155);
                    ofCircle(xPos, yPos-circleDia/2, smallCircleDia);
                    ofCircle(xPos, yPos+circleDia/2, smallCircleDia);
                    ofSetColor(255);
                    ofCircle(xPos, yPos, circleDia);
                    
                }
                
                counter++;
               
            }
            else{
                synthesisComplete =TRUE;   
                counter=0;
            }
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
       
    }    
  
    if(whichAnalysis=="M_CODE"){
        
        if(synthesisComplete == FALSE ){
        
        //cout<<"showing morse "<<morseMessage<<"\r";
        showMorse(morseMessage);
        
        //WHERE in here do we grab frames from the camera? 
        // trying to grab one everytime through - see what we get... 
        vectorOfPixels.push_back(pixels);
        
        if (morseComplete == TRUE)
        {
            string fileName; 
            for (i = 0; i < vectorOfPixels.size(); i++)
            {
                fileName = whichAnalysis+"_"+ofToString(i)+".jpg";
                ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                //cout<<i<<"< i in M_CODE ** frames being written to images \n";
            }
            vectorOfPixels.clear(); //empty out the vector
            counter = 0;
            frameCounter = 0;
            scanLinePosition=0;
            synthesisComplete=TRUE; 
            cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
        }        

        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
      //  synthesisComplete =TRUE;

    }

    //CAM_FRAMERATE 
    
    if(whichAnalysis=="CAM_FRAMERATE"){
        
        if(synthesisComplete == FALSE ){      
    
            strobe();
                        
            //there should be a saving function - that takes frames in sync with the strobe (1 frame when on, 1 frame when off)  
            //i don't understand how to do this with the strobe() encapsulated in a function like this 
            
            //do we use strobe() elsewhere?  or could we just put the guts of the function here so we can save a frame on the up and downswing of the strobe? 
            
            //The basics for file saving are below... 

            vectorOfPixels.push_back(pixels);
            
            if (strobeComplete == TRUE)
            {
                /*
                string fileName; 
                for (i = 0; i < vectorOfPixels.size(); i++)
                {
                    fileName = whichAnalysis+"_"+ofToString(i)+".jpg";
                    ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                    //cout<<i<<"< i in M_CODE ** frames being written to images \n";
                }
    
                vectorOfPixels.clear(); //empty out the vector
                counter = 0;
                frameCounter = 0;
                scanLinePosition=0;
                synthesisComplete=TRUE; 
                cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                */
            }        
            
                       
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        
        
        //synthesisComplete =TRUE;

    }
    
    //Writes a given grey value to the screen, then grabs 'X' number of grames while that grey value is on the screen     
    if(whichAnalysis=="CAM_NOISE"){
        
        if(synthesisComplete==FALSE){    ///may have to add a little thing in here that ensures the camera buffer (of frames) is empty
  
            counter++;
            
            greyValue = 255.0 - ((255.0/numberOfGreyLevels)*(int)((numberOfGreyLevels)*(double)counter/255.0));
    
            // white impulse 
            ofSetColor(greyValue, greyValue, greyValue);
        
            //cout<<counter<<" <-- CAM_NOISE COUNTER: \n";
            //cout<<((255.0/counter2)*(int)((counter2+1)*(double)counter/255.0))<<" <-- CAM_NOISE math thing \n";
            
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
            
            //cout << greyValue-oldGreyValue<<" <-- CAM_NOISE greyValue-oldGreyValue \n";
            //cout << greyValue<<" <-- CAM_NOISE greyValue \n";
            
            //whenever the greyValue is the same, take X images 
                        
            if (greyValue - oldGreyValue == 0)  {
                
                if ((localFrameCounter < framesPerGreyValue) && (k==1))
                {
                    vectorOfPixels.push_back(pixels); 
                    lightLevels.push_back(greyValue);
                    localFrameCounter++;
                    k=1;
                } else {
                    localFrameCounter=0;
                    k=0;
                }
            }  else {
                k = 1; 
            }
            
            oldGreyValue = greyValue;
            
            //once we've finished synthesis and capturing all the frames into RAM, we can then write the
            //image vectors "imgs" backinto a quicktime movie...
            
            if(counter >= 255) {
                
                string fileName; 
                
                for (i = 0; i < vectorOfPixels.size(); i++)
                {
                  //  cout<<i<<" <<-- i inside CAM_NOISE \n";
                    fileName = whichAnalysis+"_"+ofToString(i)+"_"+ofToString(lightLevels[i],2)+".jpg";
                    ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                }
                vectorOfPixels.clear(); //empty out the vector
                counter = 0;
                frameCounter = 0;
                counter=0;
                synthesisComplete=TRUE; 
                cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                 
            }
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
    }
   

    //Writes a color to the screen and reads back X number of frames
    if(whichAnalysis=="COLOR_SINGLE"){
        
        if(synthesisComplete==false){    ///may have to add a little thing in here that ensures the camera buffer (of frames) is empty
            counter++;
            cout<<counter<<"<<-- counter COLOR_SINGLE \n";
            // cout<<counterMax/3<<"<<- counterMax/3 COLOR_SINGLE \n";
            
            if (0 <= counter && counter < 100)  {
                //cout<<"counter < counterMax/3 \n";
                //red 
                ofSetColor(255, 0, 0);
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                
                if ( (localFrameCounter < framesPerColourValue) && (!gotAllLocalFrames1) )
                {
                 //  if (counter%2)  // this gives us 1 and 0 alternating with each new 'pixel' load (frame)
                 // {
                    //****PROBLEM*** - for some reason this is doubling up frames - i.e.: every other frame is the same??! 
                    //solution - get every other eligible frame - valid as long as the greyvalues are the same should wor
                    
                    vectorOfPixels.push_back(pixels); 
                    localFrameCounter++;
                    cout<<frameCounter<<"<-- frameCounter \n";
                  // }
                } else {
                    gotAllLocalFrames1 = true;
                    localFrameCounter = 0;
                }
            }
            
            if (100 <= counter && counter < 200) {
                 //cout<<"counterMax/3 <= counter <= 2*counterMax/3 \n";
                // green
                
                ofSetColor(0, 255, 0);
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                
                if ( (localFrameCounter < framesPerColourValue) && (!gotAllLocalFrames2) )
                {
                    //  if (counter%2)  // this gives us 1 and 0 alternating with each new 'pixel' load (frame)
                    //  {
                    //****PROBLEM*** - for some reason this is doubling up frames - i.e.: every other frame is the same??! 
                    //solution - get every other eligible frame - valid as long as the greyvalues are the same should wor
                    vectorOfPixels.push_back(pixels); 
                    localFrameCounter++;
                    cout<<frameCounter<<"<-- frameCounter \n";
                    
                    // }
                } else {
                    gotAllLocalFrames2 = true;
                    localFrameCounter = 0;
                }
            } 
            
            if (200 <= counter && counter < 300) {
                // blue
                ofSetColor(0, 0, 255);
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                
                if ( (localFrameCounter < framesPerColourValue) && (!gotAllLocalFrames3) )
                {
                    //  if (counter%2)  // this gives us 1 and 0 alternating with each new 'pixel' load (frame)
                    //  {
                    //****PROBLEM*** - for some reason this is doubling up frames - i.e.: every other frame is the same??! 
                    //solution - get every other eligible frame - valid as long as the greyvalues are the same should wor
                    unsigned char * someLocalPixels = new unsigned char[camWidth*camHeight*3];
                    //memcpy(someLocalPixels, pixels, (camWidth*camHeight*3));
                    imgPixels.push_back(someLocalPixels);
                    localFrameCounter++;  
                    //cout<<localFrameCounter<<"<-- localFrameCounter \n";    
                    frameCounter++;
                    cout<<frameCounter<<"<-- frameCounter \n";
                    // }
                } else {
                    gotAllLocalFrames3 = true;
                    localFrameCounter = 0;
                }
            } 

            if(counter >= counterMax) {
                cout<<"counter >= counterMax in COLOR_SINGLE \n";
                string fileName;
                for (i = 0; i < vectorOfPixels.size(); i++)
                {
                    //  cout<<i<<" <<-- i inside CAM_NOISE \n";
                    fileName = whichAnalysis+"_"+ofToString(i)+".jpg";
                    ofSaveImage(vectorOfPixels[i], fileName, OF_IMAGE_QUALITY_BEST);
                }
                
                vectorOfPixels.clear(); //empty out the vector
                counter = 0;
                frameCounter = 0;
                counter=0;
                synthesisComplete=TRUE; 
                cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";

            }
            
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        
        
    }
    

    
    //skkpping this one for the moment...
    if(whichAnalysis=="PHYS_TEST"){
        
        if(synthesisComplete == FALSE ){    
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        
        //synthesisComplete =TRUE;
    }
    
    
    //This will record as many frames as possible at specific output light hues

    if(whichAnalysis=="COLOR_MULTI"){
        
        if(synthesisComplete == FALSE ){    
               
            cHue = 255.0*(float)counter/(float)counterMax; 

            //ofColor.setHsb(float hue, float saturation, float brightness)
            aColour.setHsb(cHue, 255., 255.); 
            
            counter++;
            cout<<counter<<" <<-- counter in COLOR_MULTI \n";
            
            //ofSetColor(255.0-(255.0*(counter/300.0)), abs((255.0*counter/300.0)-255.0), 255.0*(counter/300.0)); 

            ofSetColor(aColour); 
            //cout<<aColour<<"<<-- aColour in COLOR_MULTI \n";
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
            
            // Want to test here if we have a 'new' hue - i.e.: 0, 1, 2,... 254, 255 
            // - and capture frames only when you have this new hue

            // How best to test "is this an integer"?
            
            // cout << ( (int)cHue/cHue ) << " <<-- (int)cHue in COLOR_MULTI \n";
            // if( ( (int)cHue%255 < 255 ) && !gotAllLocalFrames1){
            
                unsigned char * someLocalPixels = new unsigned char[camWidth*camHeight*3];
                //memcpy(someLocalPixels, pixels, (camWidth*camHeight*3));  
                imgPixels.push_back(someLocalPixels);
                localFrameCounter++;
                gotAllLocalFrames1 = true;                
            
            // } else {
                
            //    gotAllLocalFrames1 = false;
                
            // }
                    
            if (counter >= counterMax) {  
                
                //cout << counter << " <<-- counter >= counterMax in COLOR_MULTI \n";
                
                for (i = 0; i < localFrameCounter; i++)  
                {   
                    //cout<<i<<"< i in COLOR_MULTI ** frames being written to images \n";
                    cameraCapture.setFromPixels(imgPixels[i], camWidth, camHeight, OF_IMAGE_COLOR, true);
                    cameraCapture.saveImage(whichAnalysis+"_"+"_"+ofToString(i)+".jpg");
                }
                
                imgPixels.clear(); //empty out the vector
                counter = 0;
                synthesisComplete = TRUE;
            }

        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }

    }

}


////////////////////////---------/////////////////////////////////////
//ANALYSE VIDEO METHODS
// the below functions run analysis on the recorded video to extract whatever information we might want... 
// the output of this could be another video file, or a live rendering of the data
void analysis::analyseInput(unsigned char * pixels){
    
    
    /*
     by calling setUseTexture(false) on the ofImage before you load it, you'll stop openFrameworks from uploading the pixels to the graphics card and save a lot of space.
     */

    
    //Analysis for the specific analyses as needed...    
    if (whichAnalysis=="H_SHADOWSCAPES") {
        
    }
    
    if (whichAnalysis=="V_SHADOWSCAPES") {
        
    }     
    
    if (whichAnalysis=="D_SHADOWSCAPES") {

    } 
    
    if (whichAnalysis=="RELAXRATE") {

    } 
    
    if (whichAnalysis=="I_RESPONSE") {

    }
    
    if (whichAnalysis=="SHAPE_SHADING") {
        
    } 
    
    if (whichAnalysis=="M_CODE") {        
        
    } 
    
    if (whichAnalysis=="CAM_FRAMERATE") {
        
    } 
    
    if (whichAnalysis=="CAM_NOISE") {
        // load a first image - take the pixel value from each subsequent frame taken at each grey value
        
        // the differenced images - create z-map - overlay on the original images
    
    } 
    
    if (whichAnalysis=="COLOR_SINGLE") {
        
    } 
    
    if (whichAnalysis=="PHYS_TEST") {
        
    }
    
    if (whichAnalysis=="COLOR_MULTI") {
      
    } 
    
    if (whichAnalysis=="DIFF_NOISE") {
        
    } 

}

////////////////////////---------/////////////////////////////////////
// DISPLAY THE RESULT OF THE ANALYSIS TO SCREEN
// the below functions run analysis on the recorded video to extract whatever information we might want... 
// the output of this could be another video file, or a live rendering of the data
void analysis::displayResult(){
    
    if(whichAnalysis=="H_SHADOWSCAPES"){ 
        player.draw(0, 0, camWidth, camHeight);
    }
    
    if(whichAnalysis=="V_SHADOWSCAPES"){ 
        player.draw(0, 0, camWidth, camHeight);
    }
    
    if(whichAnalysis=="D_SHADOWSCAPES"){ 
        player.draw(0, 0, camWidth, camHeight);
    }
}


//Setup the movie for playback
void analysis::setupMovie(){
        if(!player.isLoaded()){
            player.loadMovie(cameraMovieName);
            player.play();
        }
}

//below isn't being used - but could pull the 'setup' functions from analysis setup above into this we want...
void analysis::setupQuicktimeMovieRecord(int camH, int camW, int codec){
        movieFromCamera.setCodecType(codec);
        movieFromCamera.setup(camH, camW, cameraMovieName);
}

////////////////////////---------/////////////////////////////////////
void analysis::updatePlayer(){
    player.idleMovie();
}

////////////////////////---------/////////////////////////////////////
vector<ofImage> analysis::returnFrames(){
    //return buffer;
    
}

void analysis::setupGraphs(){
    showGraphA = FALSE;
    graphCounter = 0;
    dummyCounter = 0;
    intervalCounter = 0;
    intervalSpeed = 10;
    limiter = 0;
    on = false; 
    flip = 1;    
    level = 0;
    finishedGraph = FALSE;
}

//actually just to the power of a square
float analysis::exponential(float maxResult, float maxTime,  float divisions, bool showGraph){
    //removed this and returned control of variable to gui
   // divisions = 10.0;
    float mappedLightLevel;
    float threshold=maxTime/divisions;
        
    if (limiter < maxTime) {
        
        float yPos;
        float lightLevel=pow(level,2);
        // mappedLightLevel = ofMap(lightLevel, 0, pow(maxTime/2,2), 0,maxResult);
        mappedLightLevel = ofMap(lightLevel, 0, pow(maxTime/divisions,2), 0,maxResult);

        graphCounter++;
        limiter++;
        
        //cout<<graphCounter<<" <<-- graphCounter \n";
        
        if (graphCounter < maxTime/divisions) {
            level+=flip;
            //level++;
        } else {
            level-=flip;
            //level--;
        }
             
        if(graphCounter >= threshold){
            graphCounter = 0;
            cout<<"graphCounter set to zero \n";
            flip*=-1;
        }

        if(showGraph){
            float localCount=0;
            ofNoFill();
            ofSetColor(255,0,0);
            
            float xPos=ofMap(graphCounter, 0, maxTime, 0, ofGetWidth());
            ofLine(xPos, ofGetHeight()-1, xPos, ofGetHeight()-150);
            ofBeginShape();
            for(float i=0;i<maxTime;i++){
                yPos=pow(localCount,2); 
                float mappedY = ofMap(yPos,0,pow(maxTime/2,2), 0,maxResult);
                ofVertex( ofMap(i, 0, maxTime, 0, ofGetWidth()) , ofGetHeight()-mappedY);
                if (i<maxTime/2) {
                    localCount++;
                }
                else{
                    localCount--;
                }
            }
            ofEndShape();
        }   
    } else {
        finishedGraph=TRUE;
    }

    return mappedLightLevel;
}



void analysis::quadratic(float maxResult, float maxTime, float divisions, bool showGraph){
 
    if (limiter<maxTime) {
        ofNoFill();
        ofSetColor(255, 0, 0);
        ofBeginShape();
    
    for(float i=-maxTime/2; i<maxTime/2; i++) {
        
        float yPos;
        //makes it narrower
        float a=.2;
    
        float b=.2;
        
        //shifts up or down
        float c=0;
        
        float x=i;
        yPos= (a * pow(x,2)) + (b*x)+ c; 
        
        // ofVertex(ofMap(i+maxTime/2, 0,maxTime, 0, ofGetWidth()) , yPos);
        ofVertex(i+maxTime/2 , yPos);
        cout<<i+maxTime/2<<" "<<yPos<<" yPos \r";
    }
    ofEndShape();
    }
    else{
        finishedGraph=true;
    }
}



bool analysis::squareWave(float maxResult, float maxTime, float divisions, bool showGraph){
    
    bool myBoolean;
    float threshold=maxTime/(divisions+1);
    ofFill();

    if (limiter<maxTime) {
        
        graphCounter++;
        limiter++;
        //reset graphCounter and flip boolean
        //maybe change this to a modulo
        
        myBoolean = on;
        
        if(graphCounter>=threshold){
            graphCounter=0;
            on=!on;
        }
    
        if(showGraph){
            int adjust=0;
            //this is just to prove it's making a square wave ;) //it has a gradient of one pixel/height hence the "adjust" variable
            int localCounter=0;
            bool localOn=false;
            ofBeginShape();
            ofNoFill();
            ofSetColor(255, 0, 0);
            float xPos=ofMap(limiter, 0, maxTime, 0, ofGetWidth());
            ofLine(xPos, ofGetHeight()-1, xPos, ofGetHeight()-150);
            for(int i=0;i<maxTime;i++){
            
                if(localOn){
                    ofVertex(ofMap(i+adjust, 0, maxTime, 0, ofGetWidth()) , ofGetHeight()-maxResult);
                }
                else{
                    ofVertex(ofMap(i+adjust, 0, maxTime, 0, ofGetWidth()) , ofGetHeight()-1);
                }
                localCounter++;
                adjust=0;
                if(localCounter>=threshold){
                    localCounter=0;
                    localOn=!localOn;
                    adjust=-1;
                }
            }  
            ofEndShape();
        }
        
    } else {
        finishedGraph=true;
    }
    
    return myBoolean;
}


float analysis::linear(float maxResult, float maxTime, float divisions, bool showGraph){
    //showGraph = FALSE;
    //it should change direction at every peak or trough
    float threshold=maxTime/divisions;
    float adder=maxResult/threshold;
    
    level+=adder*flip;
    
    //cout<<limiter<<" <<-- limiter in linear \n";
    //cout<<maxTime<<" <<-- maxTime in linear \n";

    if (limiter < maxTime) {

        //cout<<level<<" <<-- level in linear \n";
        graphCounter++;
        limiter++;
        
        if(graphCounter>=threshold){
            graphCounter=0;
            flip*=-1;
        }
        
        if(showGraph){
            ofSetColor(255, 0, 0);
            ofNoFill();
            float xPos=ofMap(limiter, 0, maxTime, 0, ofGetWidth());
            ofLine(xPos, ofGetHeight()-level+50, xPos, ofGetHeight()-level-50);
            int localFlip=1;
            int localCounter=0;
            float localLevel=0;
            
            ofBeginShape();
            
            for (int i=0; i<maxTime; i++) {
                cout<<adder<<" adder \r";
                localLevel+=adder*localFlip;
                
                ofVertex(ofMap(i, 0, maxTime, 0, ofGetWidth()), ofGetHeight()-localLevel);
                localCounter++;
                if(localCounter>=threshold){
                    localCounter=0;
                    localFlip*=-1;
                    
                }
            }
            ofEndShape();
        }
    }
    else{
        finishedGraph=true;
    }
    //cout<<level<<" <<-- level in linear \n";
    return level;
}


void analysis::loadMorse(){
    string line;
    const char* filePath=ofToDataPath("morse.txt").c_str();
    ifstream myfile (filePath);
    if (myfile.is_open())
    {
        while (myfile.good() )
        {
            getline (myfile,line);
            vector<string> twoHalves = ofSplitString(line, "\t");
            textTranslation.push_back(twoHalves[0]);
            morseCode.push_back(twoHalves[1]);
            // cout << line << endl;
        }
        myfile.close();
    }
    else{
        cout<<"can't open file \n";
    }
    for(int i=0;i<morseCode.size();i++){
        
        cout<<morseCode[i]<<" morse code is loaded\n";
    }
}

string analysis::translateToMorse(string messageToTranslate){
    string messageInMorse;
    cout<<messageToTranslate.length()<<" message length\r";
    for(int i=0;i<messageToTranslate.length();i++){
        //tedious casting from char to string
        stringstream ss;
        string s;
        char c = messageToTranslate.at(i);
        ss << c;
        ss >> s;
        cout<<s<<" \r";
        //string thisChar=stringmessageToTranslate.at(i);
        if(s=="_"){
            cout<<"found space\r";
            //put a forward slash in to define word break as opposed to break between characters
            messageInMorse+="/";
        }
        else{
            for(int j=0;j<morseCode.size();j++){
                if(s==textTranslation[j]){
                    //add this morse character to the message
                    messageInMorse+=morseCode[j]+"*";   
                }
            }
        }     
    }
    cout<<messageInMorse<<" message in morse \r";
    
    return messageInMorse;
}


void analysis::showMorse(string message){
    
    if(counter<message.length()){
   // graphCounter++;
   
    if(pauseBetween<=0){
        cout<<"pause bettween is "<< pauseBetween  <<" "<<message.at(counter)<< " COUNTER IS "<<counter<<" message at counter \r";
        
        cout<<" on counter "<<onCounter<<" offCounter "<<offCounter<<"\r";
        //speed=5;
        int thresh;
        if(message.at(counter)=='.'){
            thresh=speed;
            onCounter++;
            on=true;
            
        }
        
        if(message.at(counter)=='-'){
            thresh=3*speed;
            onCounter++;
            on=true;
            
        }
        
        //char break
        if(message.at(counter)=='*'){
            thresh=2*speed;
            //cout<<"got star\r";
            offCounter++;
            on=false;
            
        }
        
        //word break
        if(message.at(counter)=='/'){
            thresh=6*speed;
            offCounter++;
            on=false;
            
        }
        
        if(onCounter>=thresh){
       
            onCounter=0;
            counter++;
            pauseBetween = speed;
        
        }
        if(offCounter>=thresh){
        
            offCounter=0;
            counter++;
           // pauseBetween=thresh;
        }
        cout<<on<<" on\r";
        if(on){   
            ofSetColor(255);
        }
        else{
            ofSetColor(0);
        }
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
    }
       
    //if we just finished one character pause for one beat
    else{
        //cout<<" pausing "<<pauseBetween<<"\r";
       pauseBetween--;
    }
    }
    else{
        cout<<"finished morse\r";
        morseComplete=true;
    }
}

float analysis::returnGaussian(float x, float spread, float height,   float centre, float range){
    float returnY;
    float e=2.718281828;
    float myPower=  - ( pow( (x-centre), 2) ) / (2* (spread*spread));
    returnY= - x* height * pow(e,myPower);
    return returnY/range;
}


// This should probably strobe upwards and downwards.. and give us a straightforward rate variable that can be set from a GUI - is that what howLongToShowEachFrameRateFor?
void analysis::strobe(){

    strobeCounter++;
    // int howLongToShowEachFrameRateFor=2*(31-currentSRate);
    
    int howLongToStrobeAtEachStrobeSpeed=20;
    
    //the total length of time each fR is shown for needs to be 
    // exactly divisible by that frame rate or it will do (for example) a frame and a half and be impossible to count
    
    if(strobeCounter > howLongToStrobeAtEachStrobeSpeed){
        //advance to next stroberate   
        currentSRate = intervalGenerator();
        cout<<"new frame rate is "<<currentSRate<<"\r";
        graphCounter++;
        limiter++;
        strobeCounter=0;
    }
    
    if (limiter < 200) {
        //this speed needs calibration
        strobeToggleCounter+=1;
        
        //1/30;//1/speed
        //imagine a frame rate of 30fps
        
        if(strobeToggleCounter >= 30-currentSRate){
            on=!on;
            strobeToggleCounter=0;      
        }
        
        if(on){   
            ofSetColor(255);
        }
        else{
            ofSetColor(0);
        }
        
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    else {
        strobeComplete=true;
    }    
}

float analysis::intervalGenerator(){
    float interval;
    float maxStrobeRate=ofGetFrameRate();  //TOM - why not make this equal to 'ofGetFrameRate()'?
    
    //it should change direction at every peak or trough
    float threshold=maxTimeA/2;
    
    //go up in steps which are the total distance divided by number of changes in direction
    float adder=maxStrobeRate/threshold;
    level+=adder*flip;    
    interval=level;
    intervalCounter++;
    
    if(intervalCounter >= threshold){
        intervalCounter=0;
        flip*=-1;
    }      
    return interval;
}



// TODO:  These defaults are not reflected at programme startup when the GUIs launch - i.e.:  These don't 'set' the GUI values at the beginning but should

void analysis::setGUIDefaults (){    
        // this is called in the main programme setup - in testApp.cpp - and should contain all the default values for the GUI sliders
    
    //Tom S - completed 14 nov 19:06;03 
    animationTimeLimit = 2000;
    scanLineWidth = 10;
    scanLineSpeed = 100;

    //max white value to go to
    maxResultA = 255;
    
    //how many frames to take to do it
    maxTimeA = 100;
    
    //how many times to change direction up or down - 2 divisions is one complete cycle
    divisionsA = 2;
    showGraphA = false;
    
    //morse flash rate
    speed=30;
    red=0;
    green=0;
    blue=0;
    whichGraph="EXPONENTIAL";
    morseMessage="HELLO_WORLD";
}


//CODEC LISTING 
/*
 k32AlphaGrayCodecType
k422YpCbCr10CodecType
k422YpCbCr16CodecType
k422YpCbCr8CodecType
k4444YpCbCrA8CodecType
k4444YpCbCrA8RCodecType
k444YpCbCr10CodecType
k444YpCbCr8CodecType
k48RGBCodecType
k64ARGBCodecType
kAnimationCodecType
kBaseCodecType
kCinepakCodecType
kCloudCodecType
kCMYKCodecType
kComponentVideoCodecType
kComponentVideoSigned
kComponentVideoUnsigned
kDVCNTSCCodecType
kDVCPALCodecType
kDVCProNTSCCodecType
kDVCProPALCodecType
kFireCodecType
kFLCCodecType
kGIFCodecType
kGraphicsCodecType
kH261CodecType
kH263CodecType
kIndeo4CodecType
kJPEGCodecType
kMacPaintCodecType
kMicrosoftVideo1CodecType
kMotionJPEGACodecType
kMotionJPEGBCodecType
kMpegYUV420CodecType
kPhotoCDCodecType
kPlanarRGBCodecType
kPNGCodecType
kQuickDrawGXCodecType
kRawCodecType
kSGICodecType
kSorenson3CodecType
kSorensonCodecType
kSorensonYUV9CodecType
kTargaCodecType
kTIFFCodecType
kVideoCodecType
kWaterRippleCodecType
kYUV420CodecType
        
        */

