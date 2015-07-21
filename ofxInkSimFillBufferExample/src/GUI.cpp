//
//  GUI.cpp
//  emptyExample
//
//  Created by Akira on 5/11/15.
//
//

#include "GUI.h"

void GUI::setup(ofPtr<UniformInfos> uniforms)
{
    this->uniforms = uniforms;
    
    gui.setDefaultWidth(ofGetWidth() - 20);
    gui.setup(uniforms->parameters, "settings/uniforms.xml");
    gui.loadFromFile("settings/uniforms.xml");    
    
    bGUIHide = false;
}

void GUI::draw()
{
    if (!bGUIHide)
    {
        gui.draw();
    }
}

void GUI::toggleVisible()
{
    bGUIHide = !bGUIHide;
}

void GUI::resetToDefault()
{
    uniforms->resetToSystemDefault();
}



