#include "analysis.h"


//there is a problem with re-using the same filename so this is a hacky solution
//what's the solution here?  need to clear the folder or the video seems to always come out black? 
int movieNameCounter = 0;


////////////////////////---------/////////////////////////////////////
//SETUP THE CLASS
//Composed of things this class will always need, i.e.: a way of synthesizing the outputs, recording images
//from the camera, analysing the recorded data and outputting resulting info to the screen.

////////////////////////---------/////////////////////////////////////
void analysis::setupAnalysis(int camW, int camH, int timeLimiter, string whichAnalysisPass){//, ofVideoGrabber &grabber){
    //i included an argument which is the pointer to the grabber in case this is better than passing in pixel array? not currently used

    whichAnalysis = whichAnalysisPass;
    cout<<whichAnalysis<<" WHICH ANALYSIS AT SETUP\n";
  
    counter=0;
    timeLimit=timeLimiter;
    analysisTimer=0;
    analysed=false;
    synthesisComplete=false;

    //scanLineWidth = 1;  //if i update this here the scanLineWidth slider doesn't work!  why!!!??? 
    scanLineSpeed = 10;
    
    //the name of the file will be the name of the analysis - but we always save all the files (never overwrite)
    cameraMovieName = whichAnalysis+ofToString(movieNameCounter)+".mp4";      
    movieNameCounter++;
    
    camHeight=camH;
    camWidth=camW;
    check=0;
    
    //SETUP VIDEOSAVER
    movieFromCamera.setup(camWidth, camHeight, cameraMovieName);

    //you must call listCodecs();
    movieFromCamera.listCodecs();    

    //Setups for the specific analyses as needed...    
    if (whichAnalysis=="H_SHADOWSCAPES") {
        
    }
    
    if (whichAnalysis== "V_SHADOWSCAPES") {
        
    } //etc...
    
}


////////////////////////---------/////////////////////////////////////
//SYNTH METHODS
//all methods function called "synth" are about playing initial impulses, etc., to the screen
////////////////////////---------/////////////////////////////////////

void analysis::synthUpdate(){
    
}

void analysis::synthDrawCamRecord(unsigned char * pixels){

    //cout<<whichAnalysis<<" whichAnalysis \n";

    
    //i've implemented the horizontal one with the movie quicktime saving... and the vertical one with image saving below
    //currently i can't get the files to write properly 
    //(see the note at the top of this file - always writes a black movie if there's already a file saved from a prior launch of the prog - why!?
    
    if(whichAnalysis=="H_SHADOWSCAPES"){ 
        
        if(synthesisComplete==false && movieFromCamera.bAmSetupForRecording()){    
        
            //grab a frame from the camera
            
            movieFromCamera.addFrame(pixels);

            //is there a way to test here if the pixels have actually been written before we do the next line draw?
            
            //draw the scanning bar
            ofSetColor(255, 255, 255);
            ofRect(counter, 0, scanLineWidth, ofGetHeight());
            counter+=scanLineSpeed;  
            
            if(counter>=ofGetWidth()) {
                counter=0;
                synthesisComplete=true; 
                cout<<synthesisComplete<<"synthesis and recording complete: \n";
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
            screenCapture.setFromPixels(pixels, camWidth, camHeight, OF_IMAGE_COLOR, true);
 
            //should play here with the quality / size of the image
            //greyscale, jpg probably will be fastest and all we need?  
            //but there might be a better, more raw format that will keep us from having to decompress during analysis
            screenCapture.saveImage(whichAnalysis+"_"+ofToString(counter)+".jpg");
           
            //draw the scanning bar
            ofSetColor(255, 255, 255);
            ofRect(0, counter, ofGetWidth(), scanLineWidth);
            counter+=scanLineSpeed;
            
            if(counter>=ofGetHeight()) {
                counter=0;
                synthesisComplete=true; 
                cout<<synthesisComplete<<" synthesis and recording complete: \n";
                //write to file the video/wrap up the image sequence
            }
            
        } else {
            
           cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
    }
    
    
    //The D_SHADOWSCAPES synth and record method below should load the images as an array of pixels -
    //i.e.: let's see if we can speed up the render by holding the camera data in memory
    //something like - ofImageRecorderGrid example or tracerPix_src_v1.0
    
    if(whichAnalysis=="D_SHADOWSCAPES"){
        
        if(synthesisComplete==false){    
            
            //grab a frame from the camera - passed in to this class as 'pixels'
            screenCapture.setFromPixels(pixels, camWidth, camHeight, OF_IMAGE_COLOR, true);
            
            //should play here with the quality / size of the image
            //greyscale, jpg probably will be fastest and all we need?  
            //but there might be a better, more raw format that will keep us from having to decompress during analysis
            screenCapture.saveImage(whichAnalysis+"_"+ofToString(counter)+".jpg");
            
            //draw the scanning bar
            ofSetColor(255, 255, 255);
            ofRect(0, counter, ofGetWidth(), scanLineWidth);
            counter+=scanLineSpeed;
            
            if(counter>=ofGetHeight()) {
                counter=0;
                synthesisComplete=true; 
                cout<<synthesisComplete<<" synthesis and recording complete: \n";
                //write to file the video/wrap up the image sequence
            }
            
        } else {
            
            cout<<"couldn't synth / record - either not ready or something else it wrong...\n";
        }
    }

    /*
    if (analysisTimer>timeLimit) {
        analysed=true;    
    }
    analysisTimer++;
    */
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
}



//Setup the movie for playback
void analysis::setupMovie(){
        if(!player.isLoaded()){
            player.loadMovie(cameraMovieName);
            player.play();
        }
}

void analysis::setupSaver(int camH, int camW, int codec){
        movieFromCamera.setCodecType(codec);
        movieFromCamera.setup(camH, camW, cameraMovieName);
}

////////////////////////---------/////////////////////////////////////
void analysis::updatePlayer(){
    player.idleMovie();
}

////////////////////////---------/////////////////////////////////////
void analysis::saveOut(){
    movieFromCamera.finishMovie();
}

////////////////////////---------/////////////////////////////////////
vector<ofImage> analysis::returnFrames(){
    //return buffer;
    
}
