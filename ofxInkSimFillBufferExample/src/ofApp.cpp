#include "ofApp.h"

void ofApp::setup()
{
    inkSim.setup();
    gui.setup(inkSim.getUniformInfo());

    fbo.allocate(ofGetWidth(), ofGetHeight());
    
    ofDirectory dir;
    dir.open("imgs");
    dir.listDir();
    tests.resize(dir.size());
    for (int i = 0; i < dir.size(); i++)
    {
        string path = dir.getFile(i).getAbsolutePath();
        ofLoadImage(tests.at(i), path);
    }
    dir.close();
}

void ofApp::update()
{
    inkSim.update();
}

void ofApp::draw()
{
    inkSim.draw();
    gui.draw();
    
    ofDrawBitmapStringHighlight("press 'f' or 't' to fill buffer", 10, ofGetHeight() - 60);
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
        
//        fbo.begin();
//        ofClear(0);
//        for (int i = 0; i < 100; i++)
//        {
//            int ang = ofRandom(angMin, angRange);
//            if (ang > 360)
//                ang -= 360;
//            
//            float rad = ofRandom(10, 20);
//            
//            for (int j = 0; j < 10; j++)
//            {
//                float iteRad = ofMap(j, 0, 10, rad/10, rad, true);
//                float iteAng = ang;// + ofRandom(-10, 10);
//                
//                if (iteAng < 0)
//                    iteAng += 360;
//                else if (iteAng > 360)
//                    iteAng -= 360;
//                
//                float depth = 255;
//                ofColor c = getInkColor(iteAng, 255, depth);
//                ofPushStyle();
//                ofSetColor(c);
//                ofCircle(ofRandomWidth(), ofRandomHeight(), iteRad);
//                ofPopStyle();
//            }
//        }
//        fbo.end();
        
        inkSim.begin();
        fbo.draw(0, 0);
        inkSim.end();
    }
    
    if (key == 't')
    {
        ofColor c = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
        float depth = ofRandom(50, 100);
        
        int brush = ofRandom(tests.size());
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        inkSim.begin();
        ofPushMatrix();
        ofTranslate(ofRandomWidth(), ofRandomHeight(), ofRandom(0, -1500));
        ofPushStyle();
        ofSetColor(getInkColor(c.getHueAngle(), 255, 255));
        tests.at(brush).draw(0, 0);
        ofPopStyle();
        ofPopMatrix();
        inkSim.end();
        ofSetRectMode(OF_RECTMODE_CORNER);
    }
}

void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}