#include "ofApp.h"
#include "ofxInkSim.h"
#include "ofxInkSim.h"

void ofApp::setup()
{
    inkSim.setup();
    gui.setup(inkSim.getUniformInfo());

    fbo.allocate(ofGetWidth(), ofGetHeight());
}

void ofApp::update()
{
    inkSim.update();
}

void ofApp::draw()
{
    inkSim.draw();
    gui.draw();
    
    ofDrawBitmapStringHighlight("press 'f' to fill buffer", 10, ofGetHeight() - 60);
    ofDrawBitmapStringHighlight("press 'c' to clear buffer", 10, ofGetHeight() - 40);
    ofDrawBitmapStringHighlight("press ' ' to hide gui", 10, ofGetHeight() - 20);
}

void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}

void ofApp::keyPressed(int key)
{
    if (key == '1')
    {
        inkSim.setDrawMode(ofxInkSim::INKFIX);
    }
    else if (key == '2')
    {
        inkSim.setDrawMode(ofxInkSim::INKSURF);
    }
    else if (key == '3')
    {
        inkSim.setDrawMode(ofxInkSim::INKFLOW);
    }
    else if (key == '4')
    {
        inkSim.setDrawMode(ofxInkSim::WATERFLOW);
    }
    else if (key == 'd')
    {
        inkSim.toggleDebug();
    }
    else if (key == ' ')
    {
        gui.toggleVisible();
    }
    else if (key == 'r')
    {
        gui.resetToDefault();
    }
    else if (key == 'c')
    {
        inkSim.clear();
    }
    
    if (key == 'f')
    {
        int angRange = ofRandom(100, 200);
        int angMin = ofRandom(360);
        
        fbo.begin();
        ofClear(0);
        for (int i = 0; i < 500; i++)
        {
            int ang = ofRandom(angMin, angRange);
            if (ang > 360)
                ang -= 360;
            
            float rad = ofRandom(2, 10);
            float depth = 255;//ofMap(rad, 2, 10, 255, 200, true);
            ofColor c = getInkColor(ang, 255, depth);
            ofPushStyle();
            ofSetColor(c);
            ofCircle(ofRandomWidth(), ofRandomHeight(), rad);
            ofPopStyle();
        }
        fbo.end();
        
        inkSim.begin();
        fbo.draw(0, 0);
        inkSim.end();
    }
}

void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}