#include "analysis.h"




//there is a problem with re-using the same filename so this is a hacky solution
int movieNameCounter =0;
////////////////////////---------/////////////////////////////////////
void analysis::setupAnalysis(int camW, int camH, int timeLimiter,int whichAnalysisPass){//, ofVideoGrabber &grabber){
    //i included an argument which is the pointer to the grabber in case this is better than passing in pixel array? not currently used
    cout<<whichAnalysis<<" WHICH ANALYSIS AT SETUP\n";
    counter=0;
    timeLimit=timeLimiter;
    analysisTimer=0;
    analysed=false;
    camHeight=camH;
    camWidth=camW;
    check=0;
    whichAnalysis= whichAnalysisPass;
       movieName="grab"+ofToString(movieNameCounter)+".mp4";
    //some Analyses need to save to a video file but others don't eg structured light
    if (whichAnalysis==1) {
        usesMovie=true;
    
    }
    if (whichAnalysis==2) {
        usesMovie=false;
        stripe1.loadImage("i1.png");
        stripe2.loadImage("i2.png");
        stripe3.loadImage("i3.png");
     
        //CAMERA STUFF
        camera.position(centerX, centerY, 5000);
        centerX = ofGetWidth()/2;
        centerY = ofGetHeight()/2;
        centerZ = 0;
        lerpZ=0;
    } 
    
    //SETUP VIDEOSAVER
    saver.setup(640, 480, movieName);
    //you must call l
    saver.listCodecs();
    
        if(whichAnalysis==1){
        }
    
        if(whichAnalysis==2){
            //from kyle mcdonalds structured light example
            //set up structure light
           
            finishedDisplayingStripes=false;
        }
          
    
    
}
////////////////////////---------/////////////////////////////////////
void analysis::runAnalysis( unsigned char * pixels){

    if(whichAnalysis==1){  
                //set up structure light
                
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
                            //if it aint red color it from the camera
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
   if(whichAnalysis==2){  
           if(!finishedDisplayingStripes){
            //take one grab per light phase
            
            if(analysisTimer==(timeLimit/3)-2){
                phase1.setFromPixels(pixels,camWidth,camHeight,OF_IMAGE_COLOR,true);
               
            }
            if(analysisTimer==(2*(timeLimit/3))-2){
                phase2.setFromPixels(pixels,camWidth,camHeight,OF_IMAGE_COLOR,true);
            }
            if(analysisTimer==timeLimit-2){
                phase3.setFromPixels(pixels,camWidth,camHeight,OF_IMAGE_COLOR,true);
            }
            
        }
        else{
         //uncomment this to replace grabbed images with preloaded stills
          //  phase1.loadImage("0.jpg");
          //  phase2.loadImage("1.jpg");
          //  phase3.loadImage("2.jpg");
            int srcWidth = (int) phase1.getWidth();
            int srcHeight = (int) phase1.getHeight();
         
            threePhase.setup(srcWidth, srcHeight);
                      
            threePhase.set(0, phase1.getPixels());
            threePhase.set(1, phase2.getPixels());
            threePhase.set(2, phase3.getPixels());
            
            threePhase.setDepthScale(160);
            threePhase.setDepthSkew(-26);
            threePhase.setRangeThreshold(13);
            threePhase.setOrientation(PHASE_HORIZONTAL);
            threePhase.decode();
            
            bool* mask = threePhase.getMask();
            float* depth = threePhase.getDepth();
            byte* color = threePhase.getColor();
            
            // loading colors and depths into arrays for a vbo
            vCount = srcWidth * srcHeight;
            vertices = new GLfloat[vCount * 3];
            colors = new GLubyte[vCount * 3];
            
            int target = 0;
            for(int y = 0; y < srcHeight; y++) {
                
                for(int x = 0; x < srcWidth; x++) {
                    int src = y * srcWidth + x;
                    if(!mask[src]) {
                        
                        vertices[target * 3 + 0] = x - srcWidth / 2;
                        vertices[target * 3 + 1] = y - srcHeight / 2;
                        vertices[target * 3 + 2] = depth[src];
                       // cout<<vertices[target*3 + 2]<<"vertex here is \n";
                        colors[target * 3 + 0] = color[src* 3 + 0];
                        colors[target * 3 + 1] = color[src* 3 + 1];
                        colors[target * 3 + 2] = color[src* 3 + 2];
                        //cout<<"colors at 20 is\n";
                        target++;
                    }
                }
            }
           
            //we have finished analysing
            analysed=true;
        }
       
    }
}
////////////////////////---------/////////////////////////////////////
void analysis::displayProcess(){
    //until we haven't finished
    //cout<<whichAnalysis<<" whichAnalysis \n";
    if(whichAnalysis==1){ 
    
        if(analysed==false){    
            
            //draw an erzatz scanning bar
            ofSetColor(255, 0, 0);
            ofRect(counter, 0, 50, ofGetHeight());
            counter+=5;
    
                if(counter>=ofGetWidth()) {
                        counter=0;
                }
            analysisTimer++;
        }
    //if we get to the end of the scanning cycle, flag
        if (analysisTimer>timeLimit) {
            analysed=true;    
        }
    }
    if(whichAnalysis==2){ 
       
        if(analysisTimer>0&&analysisTimer<=timeLimit/3){ 
            stripe1.draw(0,0);
        }
        if(analysisTimer>timeLimit/3&&analysisTimer<=2*(timeLimit/3)){
            stripe2.draw(0,0);
        } 
        if(analysisTimer>2*(timeLimit/3)&&analysisTimer<timeLimit){
            stripe3.draw(0,0);
        }
        if(analysisTimer>=timeLimit){
            finishedDisplayingStripes=true;
            
        }
        analysisTimer++;
        
    }
}

void analysis::setupMovie(){
    
    if(usesMovie){
        if(!player.isLoaded()){
            player.loadMovie(movieName);
            player.play();
        }
    }
    
}
void analysis::displayResult(){
     if(whichAnalysis==1){ 
         player.draw(0, 0, camWidth, camHeight);
     }
     if(whichAnalysis==2){ 
         
         if(analysed){
         
         camera.place();
         ofPushMatrix();
        cout<<vCount<<"vCount is\n";
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);    
      //   tumble according to mouse
         glRotatef(- ofGetMouseY(),1,0,0);
         glRotatef(ofGetMouseX(),0,1,0);
         glTranslatef(-ofGetWidth()/2,-ofGetHeight()/2,0);
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2); 
         glPointSize(5);
         glBegin(GL_POINTS);
           
          for(int i =0;i<vCount;i+=3){
             
              ofSetColor(colors[i],colors[i+1],colors[i+2]);
              glVertex3f(vertices[i],vertices[i+1],vertices[i+2]);
             // cout<<vertices[i]<<" vertex \n";
          }
          
         
         glEnd();
       
             
       //      phase1.draw(0,0);
         }
         ofPopMatrix();
     }
    
}
void analysis::setupSaver(int camH, int camW, int codec){
    if(usesMovie){
      
        
        saver.setCodecType(codec);
        saver.setup(camH, camW, movieName);
    }
                
    
}

////////////////////////---------/////////////////////////////////////
void analysis::updatePlayer(){
    if(usesMovie){
        player.idleMovie();
    }
    else{
        camera.lerpPosition(centerX, centerY, lerpZ, 0.1); 
        
    }
}
////////////////////////---------/////////////////////////////////////
void analysis::saveOut(){
    if(usesMovie){
        saver.finishMovie();
    }
}
////////////////////////---------/////////////////////////////////////
vector<ofImage> analysis::returnFrames(){
    //return buffer;
    
}
////////////////////////---------/////////////////////////////////////
void analysis::captureFromInput(){
    
}
