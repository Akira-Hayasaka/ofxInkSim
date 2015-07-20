//
//  ofxInkSim.cpp
//  emptyExample
//
//  Created by Akira on 5/14/15.
//
//

#include "ofxInkSim.h"

void ofxInkSim::setup(int width, int height,
                      string grainPath, string alumPath, string pinningPath)
{
    this->width = width;
    this->height = height;
    
    ofDisableArbTex();
    
    pxSize = ofVec2f::one() / ofVec2f(width, height);
    offset = pxSize;
    
    ofLoadImage(Grain, grainPath);
    ofLoadImage(Alum, alumPath);
    ofLoadImage(Pinning, pinningPath);
    ofFbo::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.textureTarget = GL_TEXTURE_2D;
    settings.internalformat = GL_RGBA32F_ARB;
    screen.allocate(settings);
    depositionBuffer.allocate(settings);
    Disorder.allocate(settings);
    SurfInk.allocate(settings);
    Misc.allocate(settings);
    VelDen.allocate(settings);
    FlowInk.allocate(settings);
    FixInk.allocate(settings);
    Dist1.allocate(settings);
    Dist2.allocate(settings);
    SinkInk.allocate(settings);
    
    depositionBuffer.begin();
    ofClear(0, 0);
    depositionBuffer.end();
    
    Disorder.begin();
    ofClear(0, 0);
    Disorder.end();
    
    // make disorder buffer
    fillDisorderBuffer();
    
    drawMode = INKFIX;
    
    ofEnableArbTex();
    
    // debug
    bDebug = false;
}

void ofxInkSim::update()
{
    Misc.getNewTex()->begin();
    block.update(width, height,
                 pxSize,
                 uniforms->advect_p,
                 uniforms->b11, uniforms->b12, uniforms->b13,
                 uniforms->b21, uniforms->b22,
                 uniforms->p1, uniforms->p2, uniforms->p3,
                 uniforms->toe_p,
                 uniforms->omega,
                 offset,
                 Misc.getOldTex()->getTextureReference(),
                 VelDen.getOldTex()->getTextureReference(),
                 FlowInk.getOldTex()->getTextureReference(),
                 FixInk.getOldTex()->getTextureReference(),
                 Disorder.getTextureReference());
    Misc.getNewTex()->end();
    Misc.swap();
    
    Dist1.getNewTex()->begin();
    collide1.update(width, height,
                    pxSize,
                    uniforms->advect_p,
                    uniforms->omega,
                    VelDen.getOldTex()->getTextureReference(),
                    Dist1.getOldTex()->getTextureReference(),
                    FlowInk.getOldTex()->getTextureReference());
    Dist1.getNewTex()->end();
    Dist1.swap();
    
    Dist2.getNewTex()->begin();
    collide2.update(width, height,
                    pxSize,
                    uniforms->advect_p,
                    uniforms->omega,
                    VelDen.getOldTex()->getTextureReference(),
                    Dist2.getOldTex()->getTextureReference(),
                    FlowInk.getOldTex()->getTextureReference());
    Dist2.getNewTex()->end();
    Dist2.swap();
    
    Dist1.getNewTex()->begin();
    stream1.update(width, height,
                   pxSize,
                   uniforms->evapor_b,
                   offset,
                   Misc.getOldTex()->getTextureReference(),
                   Dist1.getOldTex()->getTextureReference());
    Dist1.getNewTex()->end();
    Dist1.swap();
    
    Dist2.getNewTex()->begin();
    stream2.update(width, height,
                   pxSize,
                   uniforms->evapor_b,
                   offset,
                   Misc.getOldTex()->getTextureReference(),
                   Dist2.getOldTex()->getTextureReference());
    Dist2.getNewTex()->end();
    Dist2.swap();
    
    VelDen.getNewTex()->begin();
    GetVelDen.update(width, height,
                     pxSize,
                     uniforms->wf_mul,
                     uniforms->evapor, Misc.getOldTex()->getTextureReference(),
                     Dist1.getOldTex()->getTextureReference(),
                     Dist2.getOldTex()->getTextureReference(),
                     VelDen.getOldTex()->getTextureReference());
    VelDen.getNewTex()->end();
    VelDen.swap();
    
    SurfInk.getNewTex()->begin();
    InkSupply.update(width, height,
                     pxSize,
                     VelDen.getOldTex()->getTextureReference(),
                     SurfInk.getOldTex()->getTextureReference(),
                     Misc.getOldTex()->getTextureReference());
    SurfInk.getNewTex()->end();
    SurfInk.swap();
    
    SinkInk.getNewTex()->begin();
    InkXAmt.update(width, height,
                   pxSize,
                   uniforms->f1, uniforms->f2, uniforms->f3,
                   VelDen.getOldTex()->getTextureReference(),
                   Misc.getOldTex()->getTextureReference(),
                   FlowInk.getOldTex()->getTextureReference(),
                   FixInk.getOldTex()->getTextureReference());
    SinkInk.getNewTex()->end();
    SinkInk.swap();
    
    FixInk.getNewTex()->begin();
    InkXTo.update(width, height,
                  pxSize,
                  FixInk.getOldTex()->getTextureReference(),
                  SinkInk.getOldTex()->getTextureReference());
    FixInk.getNewTex()->end();
    FixInk.swap();
    
    FlowInk.getNewTex()->begin();
    InkXFr.update(width, height,
                  pxSize,
                  FlowInk.getOldTex()->getTextureReference(),
                  SinkInk.getOldTex()->getTextureReference());
    FlowInk.getNewTex()->end();
    FlowInk.swap();
    
    FlowInk.getNewTex()->begin();
    InkFlow.update(width, height,
                   pxSize,
                   uniforms->ba1, uniforms->ba2,
                   offset,
                   VelDen.getOldTex()->getTextureReference(),
                   Misc.getOldTex()->getTextureReference(),
                   Dist1.getOldTex()->getTextureReference(),
                   Dist2.getOldTex()->getTextureReference(),
                   FlowInk.getOldTex()->getTextureReference(),
                   SurfInk.getOldTex()->getTextureReference());
    FlowInk.getNewTex()->end();
    FlowInk.swap();
}

void ofxInkSim::draw()
{
    string whatdraw = "";
    if (drawMode == INKFIX)
    {
        whatdraw = "INKFIX";
        drawXYZ(FixInk.getOldTex()->getTextureReference(), 0, 0, width, height);
    }
    else if (drawMode == INKSURF)
    {
        whatdraw = "INKSURF";
        drawXYZ(SurfInk.getOldTex()->getTextureReference(), 0, 0, width, height);
    }
    else if (drawMode == INKFLOW)
    {
        whatdraw = "INKFLOW";
        drawXYZ(FlowInk.getOldTex()->getTextureReference(), 0, 0, width, height);
    }
    else if (drawMode == WATERFLOW)
    {
        whatdraw = "WATERFLOW";
        drawZ(VelDen.getOldTex()->getTextureReference(), 0, 0, width, height);
    }
    
    if (bDebug)
    {
        int w = width/8;
        int h = height/8;
        
        drawXYZ(SurfInk.getOldTex()->getTextureReference(), 0, 0, w, h);
        drawXYZ(Misc.getOldTex()->getTextureReference(), w*1, 0, w, h);
        drawXYZ(VelDen.getOldTex()->getTextureReference(), w*2, 0, w, h);
        drawXYZ(FlowInk.getOldTex()->getTextureReference(), w*3, 0, w, h);
        drawXYZ(FixInk.getOldTex()->getTextureReference(), w*4, 0, w, h);
        drawXYZ(Dist1.getOldTex()->getTextureReference(), w*5, 0, w, h);
        drawXYZ(Dist2.getOldTex()->getTextureReference(), w*6, 0, w, h);
        drawXYZ(SinkInk.getOldTex()->getTextureReference(), w*7, 0, w, h);
        
        ofDrawBitmapStringHighlight("SurfInk OLD", 0, 15);
        ofDrawBitmapStringHighlight("Misc OLD", w*1, 15);
        ofDrawBitmapStringHighlight("VelDen OLD", w*2, 15);
        ofDrawBitmapStringHighlight("FlowInk OLD", w*3, 15);
        ofDrawBitmapStringHighlight("FixInk OLD", w*4, 15);
        ofDrawBitmapStringHighlight("Dist1 OLD", w*5, 15);
        ofDrawBitmapStringHighlight("Dist2 OLD", w*6, 15);
        ofDrawBitmapStringHighlight("SinkInk OLD", w*7, 15);
        
        depositionBuffer.draw(0, h, w, h);
        Grain.draw(w*1, h, w, h);
        Alum.draw(w*2, h, w, h);
        Pinning.draw(w*3, h, w, h);
        drawXYZ(Disorder.getTextureReference(), w*4, h, w, h);
        
        drawXYZ(SurfInk.getNewTex()->getTextureReference(), 0, height - h, w, h);
        drawXYZ(Misc.getNewTex()->getTextureReference(), w*1, height - h, w, h);
        drawXYZ(VelDen.getNewTex()->getTextureReference(), w*2, height - h, w, h);
        drawXYZ(FlowInk.getNewTex()->getTextureReference(), w*3, height - h, w, h);
        drawXYZ(FixInk.getNewTex()->getTextureReference(), w*4, height - h, w, h);
        drawXYZ(Dist1.getNewTex()->getTextureReference(), w*5, height - h, w, h);
        drawXYZ(Dist2.getNewTex()->getTextureReference(), w*6, height - h, w, h);
        drawXYZ(SinkInk.getNewTex()->getTextureReference(), w*7, height - h, w, h);
        
        ofDrawBitmapStringHighlight("SurfInk NEW", 0, height - h);
        ofDrawBitmapStringHighlight("Misc NEW", w*1, height - h);
        ofDrawBitmapStringHighlight("VelDen NEW", w*2, height - h);
        ofDrawBitmapStringHighlight("FlowInk NEW", w*3, height - h);
        ofDrawBitmapStringHighlight("FixInk NEW", w*4, height - h);
        ofDrawBitmapStringHighlight("Dist1 NEW", w*5, height - h);
        ofDrawBitmapStringHighlight("Dist2 NEW", w*6, height - h);
        ofDrawBitmapStringHighlight("SinkInk NEW", w*7, height - h);
        
        ofDrawBitmapStringHighlight(whatdraw, 0, height - h - 50);
        ofDrawBitmapStringHighlight("fps:" + ofToString(ofGetFrameRate()), 0, height - h - 30);
    }
}

void ofxInkSim::stroke(vector<StrokeInfo> sis)
{
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    depositionBuffer.begin();
    ofClear(0, 0);
    ofPushStyle();
    
    for (auto& si: sis)
    {
        si.brush->dragged(si.pos.x, si.pos.y);
        ofSetColor(si.col);
        si.brush->draw(si.brushSize);
    }
    
    ofPopStyle();
    depositionBuffer.end();
    
    ofDisableBlendMode();
    
    depositeOnPaperSurface();
}

void ofxInkSim::stroke(BrashIF* brush, int x, int y, ofColor brushCol)
{
    stroke(brush, x, y, brushCol, uniforms->brushsize);
}

void ofxInkSim::stroke(BrashIF* brush, int x, int y, ofColor brushCol, float brushSize)
{
    brush->dragged(x, y);
    
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    depositionBuffer.begin();
    ofClear(0, 0);
    ofPushStyle();
    ofSetColor(brushCol);
    brush->draw(brushSize);
    ofPopStyle();
    depositionBuffer.end();
    
    ofDisableBlendMode();
    
    depositeOnPaperSurface();
}

void ofxInkSim::stopStroke(vector<StrokeInfo> sis)
{
    for (auto& si : sis)
        si.brush->released();
}

void ofxInkSim::stopStroke(BrashIF* brush)
{
    brush->released();
}

void ofxInkSim::clear()
{
    screen.begin();
    ofClear(0, 0);
    screen.end();
    depositionBuffer.begin();
    ofClear(0, 0);
    depositionBuffer.end();
    
    Misc.clear(0);
    VelDen.clear(0);
    Dist1.clear(0);
    Dist2.clear(0);
    SurfInk.clear(0);
    FlowInk.clear(0);
    FixInk.clear(0);
    SinkInk.clear(0);
}

void ofxInkSim::setDrawMode(DRAWMODE drawMode)
{
    this->drawMode = drawMode;
}

void ofxInkSim::toggleDebug()
{
    bDebug = !bDebug;
}

void ofxInkSim::depositeOnPaperSurface()
{
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    SurfInk.getNewTex()->begin();
    ofClear(0, 0);
    addpigment.update(width, height,
                      pxSize,
                      uniforms->gamma,
                      uniforms->baseMask,
                      SurfInk.getOldTex()->getTextureReference(),
                      depositionBuffer.getTextureReference(),
                      Misc.getOldTex()->getTextureReference());
    SurfInk.getNewTex()->end();
    SurfInk.swap();
    
    Misc.getNewTex()->begin();
    ofClear(0, 0);
    addwater.update(width, height,
                    pxSize,
                    uniforms->gamma,
                    uniforms->baseMask,
                    uniforms->waterAmount,
                    Misc.getOldTex()->getTextureReference(),
                    depositionBuffer.getTextureReference());
    Misc.getNewTex()->end();
    Misc.swap();
    
    ofDisableBlendMode();
}

void ofxInkSim::drawXYZ(ofTexture& texRef, float x, float y, float w, float h)
{
    screen.begin();
    ofClear(0, 0);
    getXYZ.update(width, height, pxSize, texRef);
    screen.end();
    screen.draw(x, y, w, h);
}

void ofxInkSim::drawZ(ofTexture& texRef, float x, float y, float w, float h)
{
    screen.begin();
    ofClear(0, 0);
    getZ.update(width, height, pxSize, texRef);
    screen.end();
    screen.draw(x, y, w, h);
}

void ofxInkSim::fillDisorderBuffer()
{
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    Disorder.begin();
    GAP.update(width, height, pxSize, Grain, Alum, Pinning);
    Disorder.end();
    ofDisableBlendMode();
}

ofPtr<UniformInfos> ofxInkSim::getUniformInfo()
{
    return uniforms;
}