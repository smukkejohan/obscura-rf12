#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxQTKitVideoGrabber.h"    
#include "ofxPTZControl.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    
    void mouseReleased(int x, int y, int button);
	
	float threshold;
	
    //ofVideoGrabber vidGrabber;
    ofxQTKitVideoGrabber vidGrabber; // does not support allocate
    
    ofxQTKitVideoGrabber vidGrabber2;
    
    ofxPTZControl ptzC;
    
    //cv::Mat imgMat;
    //ofImage img;
    
	ofxCv::ContourFinder contourFinder;
	bool showLabels;
    
    int mode;
    
    int followID;

};
