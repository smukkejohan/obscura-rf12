#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxQTKitVideoGrabber.h"    
#include "ofxPTZControl.h"
#include "ofxUI.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    void exit();
    
    void keyPressed(int key);
    void mouseReleased(int x, int y, int button);    
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    void setGUI1();
    ofxUICanvas *gui1;
	
    bool hideGUI;
    
    float minAreaRadius, maxAreaRadius, threshold, persistence, distance;
    
    ofxQTKitVideoGrabber vidGrabber; // does not support allocate    
    //ofxQTKitVideoGrabber vidGrabber2;
    
    ofxPTZControl ptzC;
    
	ofxCv::ContourFinder contourFinder;
    ofxCv::TrackingColorMode trackingColorMode;
	ofColor targetColor;
    
	bool showLabels;
    int mode;
    int followID;
    
    void guiEvent(ofxUIEventArgs &e);

};
