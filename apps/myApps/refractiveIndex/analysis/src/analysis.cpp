#include "analysis.h"


//there is a problem with re-using the same filename so this is a hacky solution
int movieNameCounter =0;
////////////////////////---------/////////////////////////////////////
void analysis::setupAnalysis(int camW, int camH, int timeLimiter, string whichAnalysisPass){//, ofVideoGrabber &grabber){
    //i included an argument which is the pointer to the grabber in case this is better than passing in pixel array? not currently used
    cout<<whichAnalysis<<" WHICH ANALYSIS AT SETUP\n";
  
    counter=0;
    timeLimit=timeLimiter;
    analysisTimer=0;
    analysed=false;
    synthesisComplete=false;

    scanLineWidth = 1;
    
    camHeight=camH;
    camWidth=camW;
    check=0;
    
    whichAnalysis = whichAnalysisPass;
    
    movieName="grab"+ofToString(movieNameCounter)+".mp4";
  
    if (whichAnalysis=="H_SHADOWSCAPES") {
              
    }
    
    if (whichAnalysis== "V_SHADOWSCAPES") {
        
    } 
    
    //SETUP VIDEOSAVER
    saver.setup(640, 480, movieName);

    //you must call listCodecs();
    saver.listCodecs();    
    
}

////////////////////////---------/////////////////////////////////////
// this runs an analysis on the recorded video to extract Infomation.
void analysis::runAnalysis( unsigned char * pixels){

    if(whichAnalysis=="H_SHADOWSCAPES"){  
                
            analysedFrame = new unsigned char[camWidth*camHeight*3];
            //for each pixel passed in from the camera
            
            for (int i = 0; i < camHeight*camWidth*3; i++) {
                    //if this is the red value - modulo is probably really expensive
                    if(i%3==0 && i<(camHeight*camWidth*3)-3){
                        //if its high red
                        if(pixels[i]>100&&pixels[i+1]<70&&pixels[i+2]<70){
                            //write over these 3 pixels with white
                            analysedFrame[i] = 255;
                            analysedFrame[i+1] = 255;
                            analysedFrame[i+2] = 255;
                        }
                        else{
                            //if it isn't red color it 
                            analysedFrame[i]=pixels[i];
                            analysedFrame[i+1]=pixels[i+1];
                            analysedFrame[i+2]=pixels[i+2];
                        }
                    }
                }
                
        if(saver.bAmSetupForRecording()){
            saver.addFrame(analysedFrame);
        }
        
        else{ 
            cout<<"not ready to record\n";
        }
        
    }
    
    if(whichAnalysis=="V_SHADOWSCAPES"){  
       
        analysedFrame = new unsigned char[camWidth*camHeight*3];
        //for each pixel passed in from the camera
        
        for (int i = 0; i < camHeight*camWidth*3; i++) {
            //if this is the red value - modulo is probably really expensive
            if(i%3==0 && i<(camHeight*camWidth*3)-3){
                //if its high red
                if(pixels[i]>100&&pixels[i+1]<70&&pixels[i+2]<70){
                    //write over these 3 pixels with white
                    analysedFrame[i] = 255;
                    analysedFrame[i+1] = 255;
                    analysedFrame[i+2] = 255;
                }
                else{
                    //if it isn't red color it 
                    analysedFrame[i]=pixels[i];
                    analysedFrame[i+1]=pixels[i+1];
                    analysedFrame[i+2]=pixels[i+2];
                }
            }
        }
        
        if(saver.bAmSetupForRecording()){
            saver.addFrame(analysedFrame);
        }
        
        else{ 
            cout<<"not ready to record\n";
        }
    
    
    
    
    }
    
}


//all methods function called "synth" are about playing initial impulses, etc., to the screen


////////////////////////---------/////////////////////////////////////
void analysis::synthDraw(){
    //until we haven't finished
    //cout<<whichAnalysis<<" whichAnalysis \n";
    
    if(whichAnalysis=="H_SHADOWSCAPES"){ 
        
        if(analysed==false){    
            
            //draw the scanning bar
            ofSetColor(255, 255, 255);
            ofRect(counter, 0, scanLineWidth, ofGetHeight());
            counter+=1;  //scanLineSpeed variable to add 

            if(counter>=ofGetWidth()) {
                counter=0;
                synthesisComplete=true; 
                cout<<synthesisComplete<<"synthesisComplete flag: \n";
            }
        }
    }
    
    if(whichAnalysis=="V_SHADOWSCAPES"){

        if(analysed==false){    
      
            //draw the scanning bar
            ofSetColor(255, 255, 255);
            ofRect(0, counter, ofGetWidth(), scanLineWidth);
            counter+=1;
            
            if(counter>=ofGetHeight()) {
                counter=0;
                synthesisComplete=true; 
            }
        }      
    }

    /*
    
    if (analysisTimer>timeLimit) {
        analysed=true;    
    }
    
    analysisTimer++;
    */
}


void analysis::setupMovie(){
        if(!player.isLoaded()){
            player.loadMovie(movieName);
            player.play();
        }
}


void analysis::displayResult(){
     
    if(whichAnalysis=="H_SHADOWSCAPES"){ 
        
         player.draw(0, 0, camWidth, camHeight);
 
    }
    
    if(whichAnalysis=="V_SHADOWSCAPES"){ 
         
        player.draw(0, 0, camWidth, camHeight);
          
    }
}

void analysis::setupSaver(int camH, int camW, int codec){
        saver.setCodecType(codec);
        saver.setup(camH, camW, movieName);
}

////////////////////////---------/////////////////////////////////////
void analysis::updatePlayer(){
    player.idleMovie();
}

////////////////////////---------/////////////////////////////////////
void analysis::saveOut(){
    saver.finishMovie();
}
////////////////////////---------/////////////////////////////////////
vector<ofImage> analysis::returnFrames(){
    //return buffer;
    
}
////////////////////////---------/////////////////////////////////////
void analysis::captureFromInput(){
    
}
