#include "ofApp.h"
#include "ofxInkSim.h"
#include "ofxInkSim.h"

void ofApp::setup()
{
    inkSim.setup(&brush);
    gui.setup(inkSim.getUniformInfo());
    moodmachine.setSpeed(1.0);
}

void ofApp::update()
{
    inkSim.update();
    gui.update();
}

void ofApp::draw()
{
    inkSim.draw();
    gui.draw();
}

void ofApp::exit()
{
    gui.exit();
}

void ofApp::mouseDragged(int x, int y, int button)
{
    inkSim.stroke(x, y, moodmachine);
}

void ofApp::mouseReleased(int x, int y, int button)
{
    inkSim.stopStroke();
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
    else if (key == 'f')
    {
        gui.resetToSavedFile();
    }
    else if (key == 's')
    {
        gui.saveToFile();
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