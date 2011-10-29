//
// ofCvOpticalFlowLK.h - a OpenCV cvOpticalFlowLK warpper for openFrameworks
//
// Copyright (C) 2008 Takashi Maekawa <takachin@generative.info> 
//     All rights reserved.
//     This is free software with ABSOLUTELY NO WARRANTY.
//
// You can redistribute it and/or modify it under the terms of 
// the GNU Lesser General Public License.
//

#include "ofCvOpticalFlowLK.h"

ofCvOpticalFlowLK::ofCvOpticalFlowLK(void)
{
	captureWidth = DEFAULT_CAPTURE_WIDTH;
	captureHeight = DEFAULT_CAPTURE_HEIGHT;

	captureColsStep = DEFAULT_CAPTURE_COLS_STEP;
	captureRowsStep = DEFAULT_CAPTURE_ROWS_STEP;
}

ofCvOpticalFlowLK::~ofCvOpticalFlowLK(void)
{
	cvReleaseImage(&vel_x);
	cvReleaseImage(&vel_y);
}
	
void ofCvOpticalFlowLK::allocate(int _w, int _h){
	captureWidth = _w;
	captureHeight = _h;

	vel_x = cvCreateImage( cvSize( captureWidth ,captureHeight ), IPL_DEPTH_32F, 1  );
	vel_y = cvCreateImage( cvSize( captureWidth ,captureHeight ), IPL_DEPTH_32F, 1  );
	
    cvSetZero(vel_x);
    cvSetZero(vel_y);
}

void ofCvOpticalFlowLK::setCalcStep(int _cols, int _rows){
	captureColsStep = _cols;
	captureRowsStep = _rows;
}

void ofCvOpticalFlowLK::calc( ofxCvGrayscaleImage & pastImage,
					   ofxCvGrayscaleImage & currentImage,					   
					   int size
					   )
{
	cvCalcOpticalFlowLK( pastImage.getCvImage(), currentImage.getCvImage(),
		cvSize( size, size), vel_x, vel_y );
}

void ofCvOpticalFlowLK::draw(void){

	ofEnableAlphaBlending();
	ofSetColor(0xffffff);
	ofNoFill();

	int x, y, dx, dy;
	for ( y = 0; y < captureHeight; y+=captureRowsStep ){
		for ( x = 0; x < captureWidth; x+=captureColsStep ){

			dx = (int)cvGetReal2D( vel_x, y, x );
			dy = (int)cvGetReal2D( vel_y, y, x );

			ofLine(x,y,x+dx,y+dy);

		}
	}
	ofDisableAlphaBlending();
}
