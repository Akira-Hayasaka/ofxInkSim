#include "ofApp.h"

void ofApp::setup()
{
    inkSim.setup();
    gui.setup(inkSim.getUniformInfo());
    moodmachine.setSpeed(1.0);
}

void ofApp::update()
{
    inkSim.update();
}

void ofApp::draw()
{
    inkSim.draw();
    gui.draw();
    
    ofDrawBitmapStringHighlight("drag mouse to stroke", 10, ofGetHeight() - 40);
    ofDrawBitmapStringHighlight("press ' ' to hide gui", 10, ofGetHeight() - 20);
}

void ofApp::mouseDragged(int x, int y, int button)
{
    ofColor c = moodmachine;
    inkSim.stroke(&brush, x, y, getInkColor(c.getHueAngle(), 255, 10));
}

void ofApp::mouseReleased(int x, int y, int button)
{
    inkSim.stopStroke(&brush);
}

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
}

void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}