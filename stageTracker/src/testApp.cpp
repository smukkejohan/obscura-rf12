#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
    
    ofEnableSmoothing(); 
	ofBackground(0);    
    
    setGUI1();
    gui1->setDrawBack(false);
    
    // set defaults
    
    width = 1280;
    height = 720;
    
    vidGrabber.listDevices();
    vidGrabber.setDeviceID(11); //11
    vidGrabber.initGrabber(width,height);
	    
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(120);
	contourFinder.setThreshold(100);

	contourFinder.getTracker().setPersistence(30);
	// an object can move up to x pixels per frame
	contourFinder.getTracker().setMaximumDistance(400);
	
    contourFinder.setInvert(false);
	trackingColorMode = TRACK_COLOR_RGB;
    
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
        
        int pan = ofMap((width/2)-mouseX,-(width/2), (width/2), 99-25, 0+25);
        int tilt = ofMap((height/2)-mouseY,-(height/2), (height/2), 0+25, 99-25);
        
        ptzC.scPanTilt(pan, tilt);
        
    } else if ( mode == 2 ) {
        
        // check if we still have our target
        if (tracker.existsCurrent(followID)) {
            cv::Rect& targ = tracker.getCurrent(followID);
            
            // get the center of the bounding box
            ofVec2f pos(targ.x + targ.width / 2, targ.y + targ.height / 2);
            
            int pan = ofMap((width/2)-pos.x,-(width/2), (width/2), 99-25, 0+25);
            int tilt = ofMap((height/2)-pos.y,-(height/2), (height/2), 0+25, 99-25);
            
            ptzC.scPanTilt(pan, tilt);
      
        } else {
            ptzC.scPanTilt(50, 50);
            ptzC.scZoom(50);
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
    else if(name == "Invert")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
		invert = toggle->getValue(); 
	}
    else if(name == "Tracking color mode") {
        ofxUIDropDownList *dropdown = (ofxUIDropDownList *) e.widget;
        
        vector<ofxUIWidget *> &selected = dropdown->getSelected();
        string mode;
        for(int i = 0; i < selected.size(); i++)
        {
            mode = selected[i]->getName(); 
        }
        if(mode == "RGB") {
            trackingColorMode = TRACK_COLOR_RGB;
        } else if (mode == "HSV") {
            trackingColorMode = TRACK_COLOR_HSV;
        }
        contourFinder.setTargetColor(targetColor, trackingColorMode);
        
    }
    
}

void testApp::keyPressed(int key) {
    
    if (key == 'q') {
        mode = 1;
    }
    
    if (key == 'x') {
        for(int i = 0; i < contourFinder.size(); i++) {
            ofPoint center = toOf(contourFinder.getCenter(i));        
            if(ofDist(center.x, center.y, mouseX, mouseY) < 30) {
                followID = contourFinder.getLabel(i);
                cout<<"selected contour: "<<ofToString(followID)<<" at pos: "<<center.x<<";"<<center.y<<endl;
                mode = 2;
                break;
            } else {
                mode = 0; 
            }
        }
    }
    
    if (key == 'c') {
        targetColor = vidGrabber.getPixelsRef().getColor(mouseX, mouseY);
        contourFinder.setTargetColor(targetColor, trackingColorMode);
    }
    
    if (key == 'r') {
        ptzC.scZoom(70);
    }
    if (key == 'f') {
        ptzC.scZoom(30);
    }
    
}



//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if (key == 'q') {
        mode = 0;
        ptzC.scPanTilt(50, 50);
    }
    
    if (key == 'r' || key == 'f') {
        ptzC.scZoom(50);
    }
}

void testApp::mouseReleased(int x, int y, int button) {
    //get contour at position x y
    
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
    gui1->addWidgetDown(new ofxUIToggle(dim, dim, false, "Invert")); 
    
    vector<string> items; 
    items.push_back("RGB"); 
    items.push_back("HSV");     
    
    gui1->addWidgetDown(new ofxUIDropDownList(200, "Tracking color mode", items,OFX_UI_FONT_SMALL));
    
    /*
    gui1->addWidgetDown(new ofxUILabel("Contour tracker", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetDown(new ofxUISpacer(length-xInit, 2));
	gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0, 255, persistence, "Persistence"));
    gui1->addWidgetDown(new ofxUISlider(length-xInit,dim, 0, 255, distance, "Distance"));
    */
    
    gui1->loadSettings("GUI/guiSettings.xml");
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
}


