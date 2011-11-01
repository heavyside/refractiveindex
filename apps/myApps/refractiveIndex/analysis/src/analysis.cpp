#include "analysis.h"

//there is a problem with re-using the same filename so this is a hacky solution
//what's the solution here?  need to clear the folder or the video seems to always come out black? 
int movieNameCounter = 0;

////////////////////////---------/////////////////////////////////////
//SETUP THE CLASS
//Composed of things this class will always need, i.e.: a way of synthesizing the outputs, recording images
//from the camera, analysing the recorded data and outputting resulting info to the screen.

////////////////////////---------/////////////////////////////////////
void analysis::setupAnalysis(int camW, int camH, int analasisTimePass, string whichAnalysisPass){//, ofVideoGrabber &grabber){
    //i included an argument which is the pointer to the grabber in case this is better than passing in pixel array? not currently used

    whichAnalysis = whichAnalysisPass;
    analysisTime = analasisTimePass;
    
    cout<<whichAnalysis<<" WHICH ANALYSIS AT SETUP\n";
  
    camHeight=camH;
    camWidth=camW;
    check=0;
    
    counter=0;
  
    synthesisComplete=false;

    //you must call listCodecs();
    movieFromCamera.listCodecs();    

    //probably good in future to have one of these for each analysis - and then have the folder name increment for each 'run' of the analysis at a given site
    dataPathName = "/Users/jamieallen/Projects/newcastle/projects/RefractiveIndexLaptop/openframeworks/refractiveindex/apps/myApps/refractiveIndex/bin/data/MEDIA/";
    ofSetDataPathRoot(dataPathName);
    
    
    //Setups for the specific analyses as needed...    
    if (whichAnalysis=="H_SHADOWSCAPES") {
        
        scanLinePosition= 0; 
        scanLineWidth = 50;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
        scanLineSpeed = 10;
        
        //SETUP VIDEOSAVER
        //the name of the file will be the name of the analysis - but we always save all the files (never overwrite)
        cameraMovieName = whichAnalysis+ofToString(movieNameCounter)+".mov";          
        movieFromCamera.setCodecType(47);   //default is kJPEGCodecType = 47 (on my computer) a
        movieFromCamera.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_HIGH);   // note that kJPEGCodecType, which has no OF_QT_SAVER_CODEC_QUALITY_LOSSLESS
                                                                                // and if you set it wrong you have to clean and rebuild
        movieFromCamera.setup(camWidth, camHeight, cameraMovieName);   
        movieNameCounter++;
    }
    
    
    if (whichAnalysis=="V_SHADOWSCAPES") {
        scanLinePosition=0; 
        scanLineWidth = 25;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
        scanLineSpeed = 10;
       
    } 
    

    if (whichAnalysis=="D_SHADOWSCAPES") {
        scanLinePosition=0; 
        scanLineWidth = 15;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
        scanLineSpeed = 10;
        
    } 
    
    
    if (whichAnalysis=="RELAXRATE") {
        //SETUP VIDEOSAVER
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
        animationTimeLimit = 3000;  //milliseconds
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
        
        
    } 

    if (whichAnalysis=="CAM_FRAMERATE") {
        
        
    } 
    
    if (whichAnalysis=="CAM_NOISE") {
        frameCounter = 0;
        framesPerGreyValue = 3;
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
            
         
            // white impulse 
            ofSetColor(255-counter, 255-counter, 255-counter);               
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
            
            //the below takes in the pixel as raw unsigned chars from the camera, 
            //stores these in a vector, until the on-screen synthesis is finished 
            //then the whole set of buffered images is written to a movie file
            
            //cout<<counter<<" <-- RELAXRATE COUNTER: \n";
            
            unsigned char * someLocalPixels = new unsigned char[camWidth*camHeight*3];
            memcpy(someLocalPixels, pixels, (camWidth*camHeight*3));  
            imgPixels.push_back(someLocalPixels);  
            
            counter++;
            
            
            //once we've finished synthesis and capturing all the frames into RAM, we can then write the
            //image vectors "imgs" backinto a quicktime movie...
            
            if(counter >= 255) { 
                
                //cout<<" ** counter > 255 \n";
                for (i = 0; i < counter; i++)
                {
                    //cout<<i<<"< i in RELAXRATE ** frame add counter \n";
                    movieFromCamera.addFrame(imgPixels[i]);
                }
                
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
    
    //skkpping this one for the moment...  Leave for TOM
    if(whichAnalysis=="M_CODE"){
        
        if(synthesisComplete == FALSE ){    
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        synthesisComplete =TRUE;

    }
    
    //skkpping this one for the moment...  Waiting for Tom's strobe function
    if(whichAnalysis=="CAM_FRAMERATE"){
        
        if(synthesisComplete == FALSE ){    
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        synthesisComplete =TRUE;

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
    if(whichAnalysis=="COLOR_SINGLE"){
        
        if(synthesisComplete == FALSE ){    
            
        } else {
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
        synthesisComplete =TRUE;

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

