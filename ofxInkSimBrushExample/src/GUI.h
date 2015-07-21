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
#include "ofxGui.h"
#include "ofxInkSim.h"

class GUI
{
public:
    
    void setup(ofPtr<UniformInfos> uniforms);
    void draw();
    void toggleVisible();
    void resetToDefault();
    
private:
    
    bool bGUIHide;
    
    ofxPanel gui;
    ofXml settings;
    ofPtr<UniformInfos> uniforms;
};

#endif /* defined(__emptyExample__GUI__) */
