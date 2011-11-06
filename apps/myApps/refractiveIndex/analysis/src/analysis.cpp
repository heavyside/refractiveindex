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
  
    synthesisComplete=false;

    //you must call listCodecs();
    movieFromCamera.listCodecs();    

    //probably good in future to have one of these for each analysis - and then have the folder name increment for each 'run' of the analysis at a given site
    //TOM-> JAMIE dod you mean have a different folder? if so why don't we just use the analysis names as the folder names we could also set the site name in the gui if you like and use this? we could have a folder for each site with subfolders for each analysis
    
    
    //this is the short cut to get the data path (ie location of "data folder") ofToDataPath("") so ofToDataPath("")+"MEDIA" is prob what we want here
    
  //  dataPathName = "/Users/jamieallen/Projects/newcastle/projects/RefractiveIndexLaptop/openframeworks/refractiveindex/apps/myApps/refractiveIndex/bin/data/MEDIA/";
  //  dataPathName=ofToDataPath("")+"MEDIA";
  //  ofSetDataPathRoot(dataPathName);
    
    
    //Setups for the specific analyses as needed...    
    if (whichAnalysis=="H_SHADOWSCAPES") {
        
        scanLinePosition= 0; 
        //scanLineWidth = 50;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
        //because this is called after the gui sets this - that's why ;) 
        
       // scanLineSpeed is now set in gui for all shadowscapes
      //  scanLineSpeed = 10;
        
        //SETUP VIDEOSAVER
        //the name of the file will be the name of the analysis - but we always save all the files (never overwrite)
        //cameraMovieName = whichAnalysis+ofToString(movieNameCounter)+".mov";          
         cameraMovieName = ofToString(ofGetDay())+"_"+ofToString(ofGetHours())+"_"+ofToString(ofGetMinutes())+"_"+ofToString(ofGetSeconds())+whichAnalysis+ofToString(movieNameCounter)+".mov";   
        
        movieFromCamera.setCodecType(whichCodec);
        
        //movieFromCamera.setCodecType(47);   //default is kJPEGCodecType = 47 (on my computer) a
        movieFromCamera.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_HIGH);   // note that kJPEGCodecType, which has no OF_QT_SAVER_CODEC_QUALITY_LOSSLESS
                                                                                // and if you set it wrong you have to clean and rebuild
        movieFromCamera.setup(camWidth, camHeight, cameraMovieName);   
        movieNameCounter++;
    }
    
    
    if (whichAnalysis=="V_SHADOWSCAPES") {
        scanLinePosition=0; 
        scanLineWidth = 25;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
      //  scanLineSpeed = 10;
       
    } 
    

    if (whichAnalysis=="D_SHADOWSCAPES") {
        scanLinePosition=0; 
        scanLineWidth = 15;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
       // scanLineSpeed = 10;
        
    } 
    
    
    if (whichAnalysis=="RELAXRATE") {
        //SETUP VIDEOSAVER
        //this function initalises on the variables for the graphs
        setupGraphs();
        
        //the name of the file will be the name of the analysis - but we always save all the files (never overwrite)
        cameraMovieName = whichAnalysis+ofToString(movieNameCounter)+".mov";          
        movieFromCamera.setCodecType(47);   //default is kJPEGCodecType = 47 (on my computer) a
        movieFromCamera.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_HIGH);   // note that kJPEGCodecType, which has no OF_QT_SAVER_CODEC_QUALITY_LOSSLESS
        // and if you set it wrong you have to clean and rebuild
        movieFromCamera.setup(camWidth, camHeight, cameraMovieName);   
        movieNameCounter++;

    } 
    
    if (whichAnalysis=="I_RESPONSE") {
        lastTime = ofGetElapsedTimeMillis();
        //amimationTimeLimit is now set in GUI
       // animationTimeLimit = 3000;  //milliseconds
        timeDiff = 0;
        counter = 0;
        frameCounter = 0;
        fadeTime = 3000;
        numberOfCameraImages = 10;  //the lower this number the more camera images we get per white level shown
        counter2max = 5;   //the number of grey levels we want to look at
        counter2 = counter2max; 
    }
    
    if (whichAnalysis=="SHAPE_SHADING") {
        
    
    } 

    if (whichAnalysis=="M_CODE") {
        //im going to borrow some of these variables
        setupGraphs();
        onCounter=0;
        offCounter=0;
        pauseBetween=0;
        //load morse code from text file 
        loadMorse();
        morseMessage=translateToMorse(morseMessage);
        
    } 
    if (whichAnalysis=="CAM_FRAMERATE") {
         setupGraphs();
         synthesisComplete=false;
        floatCounter=0.0000001;
        currentFRate =0;
    } 

 
    
    if (whichAnalysis=="CAM_NOISE") {
        frameCounter = 0;
        framesPerGreyValue = 3;
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

void analysis::synthDrawCamRecord(unsigned char * pixels){

    //cout<<whichAnalysis<<" whichAnalysis \n";

    //i've implemented the horizontal one with the movie quicktime saving... and the next one with image saving below
    //currently i can't get the files to write properly 
    //(see the note at the top of this file - always writes a black movie if there's already a file saved from a prior launch of the prog - why!?
    
    if(whichAnalysis=="H_SHADOWSCAPES"){ 
        
        if(synthesisComplete==false && movieFromCamera.bAmSetupForRecording()){    
        
            //grab a frame from the camera
            
            movieFromCamera.addFrame(pixels);

            //is there a way to test here if the pixels have actually been written before we do the next line draw?
            
            //draw the scanning bar
            ofSetColor(255, 255, 255);            
            ofRect(scanLinePosition, 0, scanLineWidth, ofGetWidth());
            scanLinePosition += scanLineSpeed;
            
            if(scanLinePosition >= ofGetWidth()+(scanLineSpeed+scanLineWidth)) {
                counter=0;
                scanLinePosition=0;
                synthesisComplete=true; 
                cout<<whichAnalysis<<"<<-- synthesis and recording complete: \n";
                movieFromCamera.finishMovie();
                 //write to file the video/wrap up the image sequence
            }   
            
        } else { 
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
       
    }
    
    
    //The V_SHADOWSCAPES synth and record method below loads the individual frames as images 
    
    if(whichAnalysis=="V_SHADOWSCAPES"){
                
        if(synthesisComplete==false){    
      
            //grab a frame from the camera - passed in to this class as 'pixels'
            cameraCapture.setFromPixels(pixels, camWidth, camHeight, OF_IMAGE_COLOR, true);
 
            //should play here with the quality / size of the image
            //greyscale, jpg probably will be fastest and all we need?  
            //but there might be a better, more raw format that will keep us from having to decompress during analysis
            cameraCapture.saveImage(whichAnalysis+"_"+ofToString(counter)+".jpg");
           
            //draw the scanning bar
            ofSetColor(255, 255, 255);            
            ofRect(0, scanLinePosition, ofGetWidth(),scanLineWidth);
            scanLinePosition += scanLineSpeed;
            counter++;
            
            if(scanLinePosition >= ofGetWidth()+(scanLineSpeed+scanLineWidth)) {
                counter=0;
                scanLinePosition=0;
                synthesisComplete=true; 
                cout<<whichAnalysis<<" <<-- synthesis and recording complete: \n";
                //write to file the video/wrap up the image sequence
            }
            
        } else {
            
           cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
    }
    
    
    //The D_SHADOWSCAPES synth and record method below should load the images as an array of in-RAM pixels -
    //i.e.: let's see if we can speed up the render by holding the camera data in memory
    //something like - ofImageRecorderGrid example or tracerPix_src_v1.0
    
    if(whichAnalysis=="D_SHADOWSCAPES"){
        
        if(synthesisComplete==false){    
            
            //grab a frame from the camera - passed in to this class as 'pixels'
            cameraCapture.setFromPixels(pixels, camWidth, camHeight, OF_IMAGE_COLOR, true);
            
            //this will take each new incoming ofImage and add it to the vector of images "imgs"
            //using imgs.push_back(img)
            
            imgs.push_back(cameraCapture);
            counter++;
            
            //cout<<counter<<" <-- D_SHADOWSCAPES COUNTER: \n";
            
            //draw the scanning bar
            ofSetColor(255, 255, 255); 
            //ofSetLineWidth(scanLineWidth);
            
            ofSetPolyMode(OF_POLY_WINDING_NONZERO);
            
            ofBeginShape();
                ofVertex(0,0+scanLinePosition);
                ofVertex(0,-scanLineWidth+scanLinePosition);
                ofVertex(ofGetWidth(),-ofGetHeight()+scanLinePosition);
                ofVertex(ofGetWidth(),-ofGetHeight()+scanLineWidth+scanLinePosition);
            ofEndShape();

            
            scanLinePosition += scanLineSpeed;
            //cout<<scanLinePosition<<" <-- D_SHADOWSCAPES scanLinePosition: \n";
            
            //once we've finished synthesis and capturing all the frames into RAM, we can then write the
            //image vectors "imgs" backinto files / movies...
            
            if(scanLinePosition > 2*ofGetHeight()+(scanLineSpeed+scanLineWidth)) {
                //cout<<scanLinePosition<<" <-- scanLinePosition \n";
                
                //cout<<ofGetHeight()<<" <-- ofGetHeight() \n";
                //cout<<" ** scanLinePosition > ofGetHeight() \n";
                
                for (i = 0; i <= counter; i++)
                {
                    imgs[i].saveImage(whichAnalysis+"_"+ofToString(i)+".jpg");
                }
                
                imgs.clear();  //empty out the vector
                scanLinePosition=0; //reset the scan line position
                counter=0; //reset counter
                synthesisComplete=true; //pronounce the process complete
                cout<<whichAnalysis<<" <<-- synthesis and recording complete: \n";
            }
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        
    }

    
    //The RELAXRATE synth and record method below loads the images as an array of pixels - in memory
    //then finally writes them to a quicktime movie
    
    //The analysis as a whole going to require some kind of "wait state" for us to be able to keep something on the screen 
    //for long enough to get an average overall light reading?  i.e: cosmic latte?
    
    
    //*************  I THINK THIS IS THE MOST PROMISING WAY TO DO THE FILE SAVING - although it could be heavily RAM dependent?! **********//
    if(whichAnalysis=="RELAXRATE"){
        
        if(synthesisComplete==false){    
            
            //REPLACED THIS WITH CURVE RELAXES
            // white impulse 
            //  ofSetColor(255-counter, 255-counter, 255-counter);               
            // ofRect(0, 0, ofGetWidth(), ofGetHeight());
            
            //the below takes in the pixel as raw unsigned chars from the camera, 
            //stores these in a vector, until the on-screen synthesis is finished 
            //then the whole set of buffered images is written to a movie file
            
            //cout<<counter<<" <-- RELAXRATE COUNTER: \n";
            
            cout<<"max result and maxtime "<<maxResultA<<" "<< maxTimeA<<" "<<divisionsA<<"\r";
             if(whichGraph=="LINEAR"){
             linear(maxResultA, maxTimeA, divisionsA, showGraphA);
             }
             if(whichGraph=="EXPONENTIAL"){
             exponential(maxResultA, maxTimeA, showGraphA);
             }
             if(whichGraph=="SQUARE_WAVE"){
             squareWave(maxResultA, maxTimeA, divisionsA, showGraphA);
             }
             if(whichGraph=="QUADRATIC"){
             quadratic(maxResultA, maxTimeA, divisionsA, showGraphA);
             }
    
          /*  if(whichGraph=="LINEAR"){
                linear(255, 100, 5, true);
            }
            if(whichGraph=="EXPONENTIAL"){
                exponential(255, 100, true);
            }
            if(whichGraph=="SQUARE_WAVE"){
                squareWave(255, 100, 5, true);
            }
            if(whichGraph=="QUADRATIC"){
                quadratic(255, 100, 5, true);
            }*/
            
            unsigned char * someLocalPixels = new unsigned char[camWidth*camHeight*3];
            memcpy(someLocalPixels, pixels, (camWidth*camHeight*3));  
            imgPixels.push_back(someLocalPixels);  
            
            counter++;
            
            
            //once we've finished synthesis and capturing all the frames into RAM, we can then write the
            //image vectors "imgs" backinto a quicktime movie...
            
           // if(counter >= 255) { 
            
            
            if(finishedGraph){
                cout<<"TRYING TO START WRITING OUT BUFFER\r";
                //cout<<" ** counter > 255 \n";
                for (i = 0; i < counter; i++)
                {
                    //cout<<i<<"< i in RELAXRATE ** frame add counter \n";
                    movieFromCamera.addFrame(imgPixels[i]);
                }
                cout<<"TRYING TO SAVE OUT\r";
                movieFromCamera.finishMovie();  //wrap up the movie
            
                imgPixels.clear(); //empty out the vector
                
                scanLinePosition=0;
                counter=0;
                synthesisComplete=true; 

                cout<<whichAnalysis<<" <<-- synthesis and recording complete: \n";   
            }
            
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        
    }
    
    
    //The I_RESPONSE - see google docs
    //Not quite there yet... 
    
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
            // then we 'hold' at these constant levels of 255, 254, 253 (not implemented yet - probably just a big for loop around the below 
            */
            
           if (counter2 > 0 ) {
                
                thisTime = ofGetElapsedTimeMillis(); 
                timeDiff = thisTime-lastTime; 
                //cout<<timeDiff<<"<-- timeDiff \n";
                //cout<<counter2<<"<-- counter2 \n";
           
                if (timeDiff < animationTimeLimit) {
                      
                    ofSetColor(255.0*(counter2/counter2max), 255.0*(counter2/counter2max), 255.0*(counter2/counter2max));            
                    ofRect(0, 0, ofGetWidth(), ofGetHeight());

                    //cout<<timeDiff%numberOfCameraImages<<"<-- counter%numberOfCameraImages \n";                        
                    /*
                    //this is kind of inexact as timeDiff returns semi-random numbers... what to do here instead?
                    //basically trying to get an exact number of images over the 5000 ms that the given white values is on screen
                    //e.g.: one per second is what i'm attempting
                    */  
                    counter++;
                    
                    if (counter%numberOfCameraImages == 0)  
                    {
                        unsigned char * someLocalPixels = new unsigned char[camWidth*camHeight*3];
                        memcpy(someLocalPixels, pixels, (camWidth*camHeight*3));  
                        imgPixels.push_back(someLocalPixels);
                        frameCounter++;  
                        //cout<<frameCounter<<"<-- frameCounter \n";     
                    }
                    
                } else if ((timeDiff >= animationTimeLimit) && (timeDiff <= animationTimeLimit+fadeTime) ){
                    //cout<<"<-- inside first else if \n";
                    
                    //ofSetColor((255.0*(counter2/counter2max))-(timeDiff/3000.0),(255.0*(counter2/counter2max))-(timeDiff/3000.0),(255.0*(counter2/counter2max))-(timeDiff/3000.0)); 
                    testFloat = (255.0*(counter2/counter2max))-(255.0*(counter2/counter2max)*(timeDiff-animationTimeLimit)/(fadeTime));
                    //cout<<testFloat<<"<-- (255.0*(counter2/counter2max))-255*(timeDiff/3000.0); \n";
                
                    ofSetColor(testFloat,testFloat, testFloat);                     
                    ofRect(0, 0, ofGetWidth(), ofGetHeight());
                    
                } else if (timeDiff > (animationTimeLimit+fadeTime)) {
                   // cout<<"<-- inside second else if \n";
                    ofSetColor(0, 0, 0);               
                    ofRect(0, 0, ofGetWidth(), ofGetHeight());

                    //once we've finished synthesis and capturing all the frames into RAM, we can then write the
                    //image vectors "imgs" backinto a quicktime movie...
                    
                    for (i = 0; i < frameCounter; i++)
                    {
                        //cout<<i<<"< i in I_RESPONSE ** frames being written to images \n";
                        cameraCapture.setFromPixels(imgPixels[i], camWidth, camHeight, OF_IMAGE_COLOR, true);
                        cameraCapture.saveImage(whichAnalysis+"_"+ofToString(counter2)+"_"+ofToString(i)+".jpg");
                    }
                    
                    imgPixels.clear(); //empty out the vector            
                    frameCounter=0;
                    counter=0;
                    lastTime = ofGetElapsedTimeMillis();
                    counter2--;
                }      
            } else {
                    synthesisComplete = true;
                    cout<<whichAnalysis<<" <<-- synthesis and recording complete: \n";
 
            }
    } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
    }
    }       
    
    //skkpping this one for the moment...  This is going to be complicated - hoping for help from DAVID G
    if(whichAnalysis=="SHAPE_SHADING"){
        
        if(synthesisComplete == FALSE ){    
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        synthesisComplete =TRUE;

    }    
    
  
    if(whichAnalysis=="M_CODE"){
        
        if(synthesisComplete == FALSE ){
          //  cout<<"showing morse "<<morseMessage<<"\r";
            showMorse(morseMessage);
            
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
      //  synthesisComplete =TRUE;

    }
    
    //skkpping this one for the moment...  Waiting for Tom's strobe function
    if(whichAnalysis=="CAM_FRAMERATE"){
        
        if(synthesisComplete == FALSE ){      
            strobe();
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
      

    }
    
    //skkpping this one for the moment...  
    if(whichAnalysis=="CAM_NOISE"){
        
        if(synthesisComplete==false){    
            
            //the below takes in the pixel as raw unsigned chars from the camera, 
            //stores these in a vector, until the on-screen synthesis is finished 
            //then the whole set of buffered images is written to a movie file
        
            counter2 = 5.0;  //this number sets the number of grey levels that are shown             
            greyValue = 255.0-((255.0/counter2)*(int)((counter2+1)*(double)counter/255.0));
    
            // white impulse 
            ofSetColor(greyValue, greyValue, greyValue);
        
            //cout<<counter<<" <-- CAM_NOISE COUNTER: \n";
            //cout<<((255.0/counter2)*(int)((counter2+1)*(double)counter/255.0))<<" <-- CAM_NOISE math thing \n";
            
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
            
            cout << greyValue-oldGreyValue<<" <-- CAM_NOISE greyValue-oldGreyValue \n";

            if (greyValue != oldGreyValue){  //if we have a new grey value on screen - grab three frames

                for (i=0; i < framesPerGreyValue; i++)  //number of camera frames per output grey value 
                {
                    unsigned char * someLocalPixels = new unsigned char[camWidth*camHeight*3];
                    memcpy(someLocalPixels, pixels, (camWidth*camHeight*3));  
                    imgPixels.push_back(someLocalPixels);  
                    frameCounter++;
                }
                oldGreyValue = greyValue;
            }
            
            counter++;

            //once we've finished synthesis and capturing all the frames into RAM, we can then write the
            //image vectors "imgs" backinto a quicktime movie...
            
            if(counter >= 255) {
                
                for (i = 0; i < frameCounter; i++)
                {
                    for (j = 0; j < framesPerGreyValue; j++)
                    {
                    //cout<<i<<"< i in I_RESPONSE ** frames being written to images \n";
                    cameraCapture.setFromPixels(imgPixels[i], camWidth, camHeight, OF_IMAGE_COLOR, true);
                    cameraCapture.saveImage(whichAnalysis+"_"+ofToString(j)+"_"+ofToString(i)+".jpg");
                    }
                }
                
                imgPixels.clear(); //empty out the vector            
                frameCounter=0;
                counter=0;
                synthesisComplete=true; 
                cout<<whichAnalysis<<" <<-- synthesis and recording complete: \n";   
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
        
        synthesisComplete =TRUE;
    }
    
    
    //This will record as many frames as possible at specific output light hues

    if(whichAnalysis=="COLOR_MULTI"){
        
        if(synthesisComplete == FALSE ){    
          
           
                //  aColour.setHsb(0,255,255); 
            
                //this isn't the right way to move through the hues - just testing for speed at the moment
                //need a way to slow this down - i.e.: either by changing the color more slowly or by leaving the rectangle on screen longer after color is set
                              
                counter++;
                //cout<<counter<<"<<-- counter in COLOR_MULTI \n";
                
                ofSetColor(255.0-(255.0*(counter/300.0)), abs((255.0*counter/300.0)-255.0), 255.0*(counter/300.0)); 
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
              
                /*
                unsigned char * someLocalPixels = new unsigned char[camWidth*camHeight*3];
                memcpy(someLocalPixels, pixels, (camWidth*camHeight*3));  
                imgPixels.push_back(someLocalPixels);
                 */
                
                if (counter == 300) {  
                /*
                    for (i = 0; i < counter; i++)  
                    {   
                        //cout<<i<<"< i in COLOR_MULTI ** frames being written to images \n";
                        cameraCapture.setFromPixels(imgPixels[i], camWidth, camHeight, OF_IMAGE_COLOR, true);
                        cameraCapture.saveImage(whichAnalysis+"_"+"_"+ofToString(i)+".jpg");
                    }

                    imgPixels.clear(); //empty out the vector
                 */
                    counter=0;
                }
            
            //synthesisComplete=true; 
        
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }

    }
    if(whichAnalysis=="COLOR_SINGLE"){
        if(synthesisComplete == false){
            if(counter<animationTimeLimit){
                ofSetColor(red, green, blue);
                ofRect(0, 0, ofGetWidth(), ofGetHeight());   
                counter++;            
            }
            else{
                synthesisComplete=true;
                cout<<"completed colour single \r";
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
    
    if(whichAnalysis=="H_SHADOWSCAPES"){  
                      
    }
    
    if(whichAnalysis=="V_SHADOWSCAPES"){  
        
    }
        
    if(whichAnalysis=="D_SHADOWSCAPES"){  
        
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
    
    graphCounter = 0;
    limiter = 0;
    on=false;
    flip=1;    
    level=0;
    finishedGraph=false;
}

//actually just to the power of a square
void analysis::exponential(float maxResult, float maxTime,  bool showGraph){
    if (limiter<maxTime) {
        ofFill();
        float yPos;
        
        float lightLevel=pow(level,2);
        float mappedLightLevel = ofMap(lightLevel,0,pow(maxTime/2,2), 0,maxResult);
        graphCounter++;
        limiter++;
        if (graphCounter< maxTime/2) {
            level++;
        }
        else{
            level--;
        }
        
        ofSetColor(mappedLightLevel);
        ofRect(0,0,ofGetWidth(),ofGetHeight());
        
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
    }
    else{
        
        finishedGraph=true;
    }
}
void analysis::quadratic(float maxResult, float maxTime, float divisions, bool showGraph){
    if (limiter<maxTime) {
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofBeginShape();
    for(float i=-maxTime/2;i<maxTime/2;i++){
        
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
void analysis::squareWave(float maxResult, float maxTime, float divisions, bool showGraph){
    float threshold=maxTime/divisions;
    ofFill();
    if (limiter<maxTime) {
        
        if(on){
            ofSetColor(maxResult);
        }
        else{
            ofSetColor(0);
        }
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        
        
        
        graphCounter++;
        limiter++;
        //reset graphCounter and flip boolean
        //maybe change this to a modulo
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
    }
    else{
        
        finishedGraph=true;
    }
}

void analysis::linear( float maxResult, float maxTime, float divisions, bool showGraph){
    
    //it should change direction at every peak or trough
    float threshold=maxTime/divisions;
    
    
    float adder=maxResult/threshold;
    level+=adder*flip;
    ofFill();
    
    if (limiter<maxTime) {
        ofSetColor(level);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        
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
                
                ofVertex( ofMap(i, 0, maxTime, 0, ofGetWidth()) ,  ofGetHeight()- localLevel);
                
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
    
}

void analysis::loadMorse(){
    string line;
    const char* filePath=ofToDataPath("morse.txt").c_str();
    ifstream myfile (filePath);
    if (myfile.is_open())
    {
        while ( myfile.good() )
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
        
        cout<<morseCode[i]<<" morse\n";
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
  //  for(int i=0;i<message.length();i++){
    if(counter<message.length()){
   // graphCounter++;
        
   
    if(pauseBetween<=0){
        cout<<"pause bettween is "<<pauseBetween  <<" "<<message.at(counter)<< " COUNTER IS "<<counter<<" message at counter \r";
        
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
            cout<<"got star\r";
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
            pauseBetween=speed;
        
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
        cout<<" pausing "<<pauseBetween<<"\r";
       pauseBetween--;
        
    }
    }
    else{
        cout<<"finished morse\r";
        synthesisComplete=true;
    }
}

float analysis::returnGaussian(float x, float spread, float height,   float centre, float range){
    float returnY;
    
    float e=2.718281828;
    
    float myPower=  - ( pow( (x-centre), 2) ) / (2* (spread*spread));
    
    returnY= - x* height * pow(e,myPower);
    return returnY/range;
}

void analysis::strobe(){
    counter++;
    
   // int howLongToShowEachFrameRateFor=2*(31-currentFRate);
     int howLongToShowEachFrameRateFor=60;
    //the total length of time each fR is shown for needs to be 
    // exactly divisible by that frame rate or it will do (for example) a frame and a half and be impossible to count
    if(counter>howLongToShowEachFrameRateFor){
        //advance to next framerate   
        currentFRate = getRamp();
        cout<<"new frame rate is "<<currentFRate<<"\r";
        graphCounter++;
        limiter++;
        counter=0;
    }
    
    if (limiter<100) {
  //this speed needs calibration
        floatCounter+=1;//1/30;//1/speed
        //imagine a frame rate of 30fps
        if(floatCounter>=30-currentFRate){
            on=!on;
            floatCounter=0;      
        }
  
        if(on){   
            ofSetColor(255);
        }
        else{
            ofSetColor(0);
        }
  
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        
        
    }
    else{
        synthesisComplete=true;
    }

}
float analysis::getRamp (){
    float ramp;
    float maxTime=100; 
    
    
    float maxFrameRate=30;
    float maxResult=maxFrameRate; 
    
    //it should change direction at every peak or trough
    float threshold=maxTime/2;
    
    //go up in steps which are the total distance divided by number of changes in direction
    float adder=maxResult/threshold;
    
    level+=adder*flip;

   // if (limiter<maxTime) {
        ofSetColor(level);
        ramp=level;
        //graph counter is our position on the x axis
       // graphCounter++;
        //limiter is the overall counter 
       // limiter++;
        
        
        if(graphCounter>=threshold){
            graphCounter=0;
            flip*=-1;
        }
      
    return ramp;
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

