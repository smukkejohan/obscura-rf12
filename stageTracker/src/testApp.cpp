#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(0);
	    
    vidGrabber.setDeviceID(11);
    vidGrabber.initGrabber(1280,720);
	
    vidGrabber2.setDeviceID(<#int videoDeviceID#>)
    
	contourFinder.setMinAreaRadius(2);
	contourFinder.setMaxAreaRadius(80);
	contourFinder.setThreshold(150);
	// wait for half a frame before forgetting something
	contourFinder.getTracker().setPersistence(15);
	// an object can move up to x pixels per frame
	contourFinder.getTracker().setMaximumDistance(400);
	
	showLabels = true;
    mode = 0;
    
    ptzC.init();    
}

void testApp::update() {
	vidGrabber.update();
	if(vidGrabber.isFrameNew()) {		         
        ofImage img;
        img.setFromPixels(vidGrabber.getPixelsRef());
        blur(img, 30);
        contourFinder.findContours(img);
        
	}
}

void testApp::draw() {
	ofSetBackgroundAuto(showLabels);
	RectTracker& tracker = contourFinder.getTracker();
	
	if(showLabels) {
		ofSetColor(200);
		vidGrabber.draw(0, 0);
		contourFinder.draw();
		for(int i = 0; i < contourFinder.size(); i++) {
			ofPoint center = toOf(contourFinder.getCenter(i));
			ofPushMatrix();
			ofTranslate(center.x, center.y);
			int label = contourFinder.getLabel(i);
            
            if(label == followID && mode == 2) {
                ofSetColor(255, 0, 0);
            } else {
                ofSetColor(200);
            }
            
			string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
			ofDrawBitmapString(msg, 0, 0);
			ofVec2f velocity = toOf(contourFinder.getVelocity(i));
			ofScale(10, 10);
			ofLine(0, 0, velocity.x, velocity.y);
			ofPopMatrix();
		}
	}
    
    if (mode == 1) {
        
        int pan = ofMap(640-mouseX,-640, 640, 99, 0);
        int tilt = ofMap(360-mouseY,-360, 360, 0, 99);
        
        ptzC.scPanTilt(pan, tilt);
    } else if ( mode == 2 ) {
        
        // check if we still have our target
        if (tracker.existsCurrent(followID)) {
            cv::Rect& targ = tracker.getCurrent(followID);
            
            // get the center of the bounding box
            ofVec2f pos(targ.x + targ.width / 2, targ.y + targ.height / 2);
            
            int pan = ofMap(640-pos.x,-640, 640, 99-20, 0+20);
            int tilt = ofMap(360-pos.y,-360, 360, 0+20, 99-20);
            
            ptzC.scPanTilt(pan, tilt);
      
        } else {
            ptzC.scPanTilt(50, 50);
            mode = 0;
        }
    }
    
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		showLabels = !showLabels;
	}
    
    if (key == 'a') {
        mode = 1;
    }
    
}

void testApp::mouseReleased(int x, int y, int button) {
    //get contour at position x y
    
    for(int i = 0; i < contourFinder.size(); i++) {
        ofPoint center = toOf(contourFinder.getCenter(i));        
        if(ofDist(center.x, center.y, x, y) < 30) {
            followID = contourFinder.getLabel(i);
            cout<<"selected contour: "<<ofToString(followID)<<" at pos: "<<center.x<<";"<<center.y<<endl;
            mode = 2;
            break;
        } else {
            mode = 0; 
        }
        
    }
    
}

