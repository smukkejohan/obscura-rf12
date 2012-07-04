#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
    
    ofEnableSmoothing(); 
	ofBackground(0);    
    
    setGUI1();
    gui1->setDrawBack(false);
    
    // set defaults
    
    vidGrabber.listDevices();
    vidGrabber.setDeviceID(0); //11
    vidGrabber.initGrabber(1280,720);
	    
	contourFinder.setMinAreaRadius(2);
	contourFinder.setMaxAreaRadius(80);
	contourFinder.setThreshold(150);
	// wait for half a frame before forgetting something
	contourFinder.getTracker().setPersistence(15);
	// an object can move up to x pixels per frame
	contourFinder.getTracker().setMaximumDistance(400);
	
    //contourFinder.setInvert(true); // find black instead of white
	trackingColorMode = TRACK_COLOR_RGB;
    
	showLabels = true;
    mode = 0;
    
    

    
    
    
    //ptzC.init();
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
    
    ofPushStyle(); 
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
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
        
        //ptzC.scPanTilt(pan, tilt);
    } else if ( mode == 2 ) {
        
        // check if we still have our target
        if (tracker.existsCurrent(followID)) {
            cv::Rect& targ = tracker.getCurrent(followID);
            
            // get the center of the bounding box
            ofVec2f pos(targ.x + targ.width / 2, targ.y + targ.height / 2);
            
            int pan = ofMap(640-pos.x,-640, 640, 99-20, 0+20);
            int tilt = ofMap(360-pos.y,-360, 360, 0+20, 99-20);
            
            //ptzC.scPanTilt(pan, tilt);
      
        } else {
            //ptzC.scPanTilt(50, 50);
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
    
    targetColor = vidGrabber.getPixelsRef().getColor(x, y);
	contourFinder.setTargetColor(targetColor, trackingColorMode);
    
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


void testApp::exit()
{
    gui1->saveSettings("GUI/guiSettings.xml");     
    delete gui1; 
}


void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl;
    
    
    if(name == "Area radius range")
	{
		ofxUIRangeSlider *rslider = (ofxUIRangeSlider *) e.widget;
        
        minAreaRadius = rslider->getScaledValueLow(); 
        maxAreaRadius = rslider->getScaledValueHigh();  
        
        contourFinder.setMinAreaRadius(minAreaRadius);
        contourFinder.setMaxAreaRadius(maxAreaRadius);
	}
    else if(name == "Threshold")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		threshold = slider->getScaledValue();
        contourFinder.setThreshold(threshold);
	}
    // tracker
    else if(name == "Persistence")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		persistence = slider->getScaledValue();
        contourFinder.getTracker().setPersistence(persistence);
	}
    else if(name == "Persistence")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		persistence = slider->getScaledValue();
        contourFinder.getTracker().setPersistence(persistence);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{ 
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}

void testApp::setGUI1()
{
	hideGUI = false; 
	
    float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit;
    
	gui1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight()); 
    
	gui1->addWidgetDown(new ofxUILabel("Contour finder", OFX_UI_FONT_MEDIUM)); 
    gui1->addWidgetDown(new ofxUISpacer(length-xInit, 2));
        
    gui1->addWidgetDown(new ofxUIRangeSlider(length-xInit,dim, 0.0, 300.0, minAreaRadius, maxAreaRadius, "Area radius range")); 	
     
	gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0, 255, threshold, "Threshold"));    
    
    gui1->addWidgetDown(new ofxUILabel("Contour tracker", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetDown(new ofxUISpacer(length-xInit, 2));
	gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0, 255, persistence, "Persistence"));
    gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0, 255, distance, "Distance"));
    
    gui1->loadSettings("GUI/guiSettings.xml");

	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
}


