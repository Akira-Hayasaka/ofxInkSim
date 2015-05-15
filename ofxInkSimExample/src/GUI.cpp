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
    resetToDefault();
    
    gui = new ofxUISuperCanvas("UNIFORMS");
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    gui->disableAppEventCallbacks();
    gui->setWidth(330);

    gui->addSpacer();
    gui->addLabel("brush size");
    gui->addSlider("brushsize", 1.0, 50.0, &uniforms->brushsize);
    
    gui->addSpacer();
    gui->addLabel("water");
    gui->addSlider("waterAmount", 0.00000, 2.00000, &uniforms->waterAmount); // water
    gui->addSpacer();
    gui->addLabel("pigment, water");
    gui->addSlider("gamma", 0.00000, 1.00000, &uniforms->gamma); // pigment, water
    gui->addSlider("baseMask", 0.00000, 1.00000, &uniforms->baseMask); // pigment, water

    gui->addSpacer();
    gui->addLabel("block");
    gui->addSlider("toe_p", 0.00000, 1.00000, &uniforms->toe_p); // block
    gui->addSpacer();
    gui->addSlider("blk1_default", 0.00000, 1.00000, &uniforms->b11); // block
    gui->addSlider("blk1_grain", 0.00000, 1.00000, &uniforms->b12); // block
    gui->addSlider("blk1_alum", 0.00000, 1.00000, &uniforms->b13); // block
    gui->addSpacer();
    gui->addSlider("blk2_glue", 0.00000, 1.00000, &uniforms->b21); // block
    gui->addSlider("blk2_fixblk", 0.00000, 1.00000, &uniforms->b22); // block
    gui->addSpacer();
    gui->addSlider("pinw_default", 0.00000, 1.00000, &uniforms->p1); // block
    gui->addSlider("pinw_fixblk", 0.00000, 1.00000, &uniforms->p2); // block
    gui->addSlider("pinw_pindisorder(g,p,step(0,toe,glue))", 0.00000, 1.00000, &uniforms->p3); // block
    gui->addSpacer();
    gui->addLabel("block, collide1, collide2");
    gui->addSlider("omega", 0.00000, 1.00000, &uniforms->omega); // block, collide1, collide2
    gui->addSlider("advect_p", 0.00000, 1.00000, &uniforms->advect_p); // block, collide1, collide2
    
//    gui->addSpacer();
//    gui->addLabel("velden");
//    gui->addSlider("wf_mul", 0.00000, 3.00000, &uniforms->wf_mul); // velden
    gui->addSpacer();
    gui->addLabel("velden, stream1, stream2");
    gui->addSlider("evapor", 0.00000, 0.01000, &uniforms->evapor); // velden, stream1, stream2
    gui->addSpacer();
    gui->addLabel("stream1, stream2");
    gui->addSlider("evapor_b", 0.00000, 0.00100, &uniforms->evapor_b); // stream1, stream2

    gui->addSpacer();
    gui->addLabel("inkflow");
    gui->addSlider("blka_lowest_hindrance_rate", 0.00000, 0.00010, &uniforms->ba1); // inkflow
    gui->addSlider("blka_blocking_advection_when_flow_speed_low", 0.00000, 0.00010, &uniforms->ba2); // inkflow
    
    gui->addSpacer();
    gui->addLabel("inkxamt");
    gui->addSlider("fixrate_base_fix_factor", 0.00000, 0.01000, &uniforms->f1); // inkxamt
    gui->addSlider("fixrate_modulating_dryness_glue1", 0.00000, 0.10000, &uniforms->f2);  // inkxamt
    gui->addSlider("fixrate_modulating_dryness_glue2", 0.00000, 0.10000, &uniforms->f3);  // inkxamt
    
    gui->autoSizeToFitWidgets();
    gui->loadSettings("settings/uniforms.xml");
    
    bGUIHide = false;
}

void GUI::update()
{
    if (!bGUIHide)
    {
        gui->update();
    }
}

void GUI::draw()
{
    if (!bGUIHide)
    {
        gui->draw();
    }
}

void GUI::toggleVisible()
{
    bGUIHide = !bGUIHide;
    
    if (!bGUIHide)
    {
        gui->enable();
    }
    else
    {
        gui->disable();
    }
}

void GUI::exit()
{
    delete gui;
}

void GUI::resetToDefault()
{
    uniforms->resetToSystemDefault();
}

void GUI::resetToSavedFile()
{
    gui->loadSettings("settings/uniforms.xml");
}

void GUI::saveToFile()
{
    gui->saveSettings("settings/uniforms.xml");
}


