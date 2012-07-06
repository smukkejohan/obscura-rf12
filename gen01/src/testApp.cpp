#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    
    ofSetWindowShape(1280, 720);
    
    ofxFenster* win=ofxFensterManager::get()->createFenster(0,0,1280,720,OF_WINDOW);
    win->addListener(this);
    win->setWindowTitle("Window 1");
    win->setBorder(false);
    
	ofBackground(255,255,255);
    ofEnableSmoothing();
	ofSetFrameRate(30);
    
    // Calculate ccoordinates for output triangles
    nTriangles = 10;
    
    int topgap = 15;
    int sidegap = 14;
    int middlegap = 17;
    int gap = 40;    
    int y = topgap;
    int x = sidegap;
    int tWidth = 388;
    int tHeight = 336;
    int tn = 0;
    // Two lines of 3 triangles facing up 
    for(int lines=0; lines<2; lines++) {
        for(int i=0; i<3; i++) {
            outTriangles[tn].x = x;
            outTriangles[tn].y = y;
            
            outTriangles[tn].A = ofVec2f(0, tHeight);
            outTriangles[tn].B = ofVec2f(tWidth/2, 0);
            outTriangles[tn].C = ofVec2f(tWidth, tHeight);
            
            
            tn += 1;
            x += tWidth + gap;
        }
        y += tHeight + middlegap; 
        x = sidegap;
    }    
    x = sidegap + (tWidth/2) + (gap/2);
    y = topgap;
    
    // Two lines of 2 triangles facing down
    for(int lines=0; lines<2; lines++) {
        for(int i=0; i<2; i++) {
            outTriangles[tn].x = x;
            outTriangles[tn].y = y;
            outTriangles[tn].A = ofVec2f(0, 0);
            outTriangles[tn].B = ofVec2f(tWidth/2, tHeight);
            outTriangles[tn].C = ofVec2f(tWidth, 0);
            tn += 1;
            x += tWidth + gap;
        }
        y += tHeight + middlegap; 
        x = sidegap + (tWidth/2) + (gap/2);
    }
    
    for (int i = 0; i < nTriangles; i++){
		
        outTriangles[i].width  = tWidth;
		outTriangles[i].height = tHeight;
        
        outTriangles[i].c = outTriangles[i].A.distance(outTriangles[i].B);
        outTriangles[i].a = outTriangles[i].B.distance(outTriangles[i].C);
        outTriangles[i].b = outTriangles[i].A.distance(outTriangles[i].C);
        
        outTriangles[i].center = (outTriangles[i].A + outTriangles[i].B + outTriangles[i].C)/3;
	}
    
    
    outTriangles[0].period = 10;
    outTriangles[1].period = 5;
    outTriangles[2].period = 8;
    outTriangles[3].period = 7;
    outTriangles[4].period = 3;
    outTriangles[5].period = 8;
    outTriangles[6].period = 2;
    outTriangles[7].period = 1;
    outTriangles[8].period = 0;
    outTriangles[9].period = 12;
    
    
    font.loadFont("Centurygothic.ttf", 32);
    
    
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	ofNoFill();
    ofBackground(0, 0, 0);
	ofSetColor(255);
    
    // draw triangles and circles in the middle
    /*for(int i=0; i<nTriangles; i++) {
        ofPushMatrix();        
        ofTranslate(outTriangles[i].x, outTriangles[i].y);
        ofTriangle(outTriangles[i].A, outTriangles[i].B, outTriangles[i].C);
        ofPopMatrix();
        
        ofSetColor(50, 50, 50);
        ofFill();
        ofCircle(outTriangles[i].center, sin(ofGetElapsedTimef()) * 20);
        ofNoFill();
        ofSetColor(255); 
    }*/
    
    // background fades
    /*ofFill(); 
    cout<<ofToString(abs(sin(ofGetElapsedTimef()) * nTriangles))<<endl;    
    for(int i=0; i<nTriangles; i++) {
        ofPushMatrix();
        
        ofSetColor(abs(sin(ofGetElapsedTimef()/outTriangles[i].period) * 100));
        
        ofTranslate(outTriangles[i].x, outTriangles[i].y);
        ofTriangle(outTriangles[i].A, outTriangles[i].B, outTriangles[i].C);
        ofPopMatrix();

    }*/
    
    // characters
    for(int i=0; i<nTriangles; i++) {
        ofPushMatrix();
        //ofSetColor(abs(sin(ofGetElapsedTimef()/outTriangles[i].period) * 100));
        
        ofTranslate(outTriangles[i].x, outTriangles[i].y);
        ofTriangle(outTriangles[i].A, outTriangles[i].B, outTriangles[i].C);
                
        int sw = font.stringWidth(ofToString(20));

        font.drawString(ofToString(i), outTriangles[i].center.x-(sw/2),outTriangles[i].center.y);
        
        ofPopMatrix();
        
    }
    
    
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key, ofxFenster* win)
{
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key, ofxFenster* win)
{
	
}

//this only works if testApp is extending ofxFensterListener and not ofBaseApp
void testApp::mouseMoved(int x, int y, ofxFenster* win)
{
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
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
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
    
}

void testApp::mouseMovedEvent(ofMouseEventArgs &args)
{
	//cout << "MOUSE WAS MOVED" << endl;
}
