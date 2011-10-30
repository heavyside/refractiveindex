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
  
    camHeight=camH;
    camWidth=camW;
    check=0;
    
    counter=0;
    scanLinePosition=0; 
    scanLineWidth = 50;  //if i initialise this here the scanLineWidth GUI slider doesn't work!  why!!!??? 
    scanLineSpeed = 10;
    
    timeLimit=timeLimiter;
    analysisTimer=0;
    
    analysed=false;
    synthesisComplete=false;

    //you must call listCodecs();
    //movieFromCamera.listCodecs();    

    //Setups for the specific analyses as needed...    
    if (whichAnalysis=="H_SHADOWSCAPES") {
        //SETUP VIDEOSAVER
        //the name of the file will be the name of the analysis - but we always save all the files (never overwrite)
        cameraMovieName = whichAnalysis+ofToString(movieNameCounter)+".mp4";          
        movieFromCamera.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);
        movieFromCamera.setup(camWidth, camHeight, cameraMovieName);   
        movieNameCounter++;
    }
    
    //doesn't use the movie thing currently
    if (whichAnalysis=="V_SHADOWSCAPES") {
       
    } //etc...

    //doesn't use the movie thing currently    
    if (whichAnalysis=="D_SHADOWSCAPES") {
        
    } //etc...
    
    
    if (whichAnalysis=="RELAXRATE") {
        //SETUP VIDEOSAVER
        //the name of the file will be the name of the analysis - but we always save all the files (never overwrite)
        cameraMovieName = whichAnalysis+ofToString(movieNameCounter)+".mp4";          
        movieFromCamera.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);
        movieFromCamera.setup(camWidth, camHeight, cameraMovieName);   
        movieNameCounter++;
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
            screenCapture.setFromPixels(pixels, camWidth, camHeight, OF_IMAGE_COLOR, true);
 
            //should play here with the quality / size of the image
            //greyscale, jpg probably will be fastest and all we need?  
            //but there might be a better, more raw format that will keep us from having to decompress during analysis
            screenCapture.saveImage(whichAnalysis+"_"+ofToString(counter)+".jpg");
           
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
            screenCapture.setFromPixels(pixels, camWidth, camHeight, OF_IMAGE_COLOR, true);
            
            //this will take each new incoming ofImage and add it to the vector of images "imgs"
            //using imgs.push_back(img)
            
            imgs.push_back(screenCapture);
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
                cout<<scanLinePosition<<" <-- scanLinePosition \n";
                
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
    
    if(whichAnalysis=="RELAXRATE"){
        
        if(synthesisComplete==false){    
            
            //grab a frame from the camera - passed in to this class as 'pixels'
            screenCapture.setFromPixels(pixels, camWidth, camHeight, OF_IMAGE_COLOR, true);
            
            //this will take each new incoming ofImage and add it to the vector of images "imgs"
            //using imgs.push_back(img)
            
            imgs.push_back(screenCapture);
            counter++;
            //cout<<counter<<" <-- D_SHADOWSCAPES COUNTER: \n";
            
            //draw the scanning bar
            ofSetColor(255, 255, 255);            
            ofRect(0, 0, ofGetWidth(), ofGetHeight());
            scanLinePosition += scanLineSpeed;
            //cout<<scanLinePosition<<" <-- D_SHADOWSCAPES scanLinePosition: \n";
            
            //once we've finished synthesis and capturing all the frames into RAM, we can then write the
            //image vectors "imgs" backinto files / movies...
            
            if(scanLinePosition > ofGetHeight()+(scanLineSpeed+scanLineWidth)) {  // makes sure that the scan line leaves the screen
                
                //cout<<ofGetHeight()<<" <-- ofGetHeight() \n";
                //cout<<" ** scanLinePosition > ofGetHeight() \n";
                
                for (i = 0; i <= counter; i++)
                {
                    imgs[i].saveImage(whichAnalysis+"_"+ofToString(i)+".jpg");
                }
                
                scanLinePosition=0;
                counter=0;
                synthesisComplete=true; 
                cout<<whichAnalysis<<" <<-- synthesis and recording complete: \n";
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
