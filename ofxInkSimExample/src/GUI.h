//
//  GUI.h
//  emptyExample
//
//  Created by Akira on 5/11/15.
//
//

#ifndef __emptyExample__GUI__
#define __emptyExample__GUI__

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxInkSim.h"

class GUI
{
public:
    
    void setup(ofPtr<UniformInfos> uniforms);
    void update();
    void draw();
    void toggleVisible();
    void exit();
    void resetToDefault();
    void resetToSavedFile();
    void saveToFile();
    
private:
    
    bool bGUIHide;
    
    ofxUISuperCanvas *gui;
    ofPtr<UniformInfos> uniforms;
};

#endif /* defined(__emptyExample__GUI__) */
