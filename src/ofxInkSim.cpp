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
    
    ofLoadImage(grain, grainPath);
    ofLoadImage(alum, alumPath);
    ofLoadImage(pinning, pinningPath);
    ofFbo::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.textureTarget = GL_TEXTURE_2D;
    settings.internalformat = GL_RGBA32F_ARB;
    screen.allocate(settings);
    depositionBuffer.allocate(settings);
    disorder.allocate(settings);
    surfInk.allocate(settings);
    misc.allocate(settings);
    velDen.allocate(settings);
    flowInk.allocate(settings);
    fixInk.allocate(settings);
    dist1.allocate(settings);
    dist2.allocate(settings);
    sinkInk.allocate(settings);
    
    depositionBuffer.begin();
    ofClear(0, 0);
    depositionBuffer.end();
    
    disorder.begin();
    ofClear(0, 0);
    disorder.end();
    
    // make disorder buffer
    fillDisorderBuffer();
    
    drawMode = INKFIX;
    
    ofEnableArbTex();
    
    // debug
    bDebug = false;
}

void ofxInkSim::update()
{
    misc.getNewTex()->begin();
    block.update(width, height,
                 pxSize,
                 uniforms->advect_p,
                 uniforms->b11, uniforms->b12, uniforms->b13,
                 uniforms->b21, uniforms->b22,
                 uniforms->p1, uniforms->p2, uniforms->p3,
                 uniforms->toe_p,
                 uniforms->omega,
                 offset,
                 misc.getOldTex()->getTextureReference(),
                 velDen.getOldTex()->getTextureReference(),
                 flowInk.getOldTex()->getTextureReference(),
                 fixInk.getOldTex()->getTextureReference(),
                 disorder.getTextureReference());
    misc.getNewTex()->end();
    misc.swap();
    
    dist1.getNewTex()->begin();
    collide1.update(width, height,
                    pxSize,
                    uniforms->advect_p,
                    uniforms->omega,
                    velDen.getOldTex()->getTextureReference(),
                    dist1.getOldTex()->getTextureReference(),
                    flowInk.getOldTex()->getTextureReference());
    dist1.getNewTex()->end();
    dist1.swap();
    
    dist2.getNewTex()->begin();
    collide2.update(width, height,
                    pxSize,
                    uniforms->advect_p,
                    uniforms->omega,
                    velDen.getOldTex()->getTextureReference(),
                    dist2.getOldTex()->getTextureReference(),
                    flowInk.getOldTex()->getTextureReference());
    dist2.getNewTex()->end();
    dist2.swap();
    
    dist1.getNewTex()->begin();
    stream1.update(width, height,
                   pxSize,
                   uniforms->evapor_b,
                   offset,
                   misc.getOldTex()->getTextureReference(),
                   dist1.getOldTex()->getTextureReference());
    dist1.getNewTex()->end();
    dist1.swap();
    
    dist2.getNewTex()->begin();
    stream2.update(width, height,
                   pxSize,
                   uniforms->evapor_b,
                   offset,
                   misc.getOldTex()->getTextureReference(),
                   dist2.getOldTex()->getTextureReference());
    dist2.getNewTex()->end();
    dist2.swap();
    
    velDen.getNewTex()->begin();
    getVelDen.update(width, height,
                     pxSize,
                     uniforms->wf_mul,
                     uniforms->evapor, misc.getOldTex()->getTextureReference(),
                     dist1.getOldTex()->getTextureReference(),
                     dist2.getOldTex()->getTextureReference(),
                     velDen.getOldTex()->getTextureReference());
    velDen.getNewTex()->end();
    velDen.swap();
    
    surfInk.getNewTex()->begin();
    inkSupply.update(width, height,
                     pxSize,
                     velDen.getOldTex()->getTextureReference(),
                     surfInk.getOldTex()->getTextureReference(),
                     misc.getOldTex()->getTextureReference());
    surfInk.getNewTex()->end();
    surfInk.swap();
    
    sinkInk.getNewTex()->begin();
    inkXAmt.update(width, height,
                   pxSize,
                   uniforms->f1, uniforms->f2, uniforms->f3,
                   velDen.getOldTex()->getTextureReference(),
                   misc.getOldTex()->getTextureReference(),
                   flowInk.getOldTex()->getTextureReference(),
                   fixInk.getOldTex()->getTextureReference());
    sinkInk.getNewTex()->end();
    sinkInk.swap();
    
    fixInk.getNewTex()->begin();
    inkXTo.update(width, height,
                  pxSize,
                  fixInk.getOldTex()->getTextureReference(),
                  sinkInk.getOldTex()->getTextureReference());
    fixInk.getNewTex()->end();
    fixInk.swap();
    
    flowInk.getNewTex()->begin();
    inkXFr.update(width, height,
                  pxSize,
                  flowInk.getOldTex()->getTextureReference(),
                  sinkInk.getOldTex()->getTextureReference());
    flowInk.getNewTex()->end();
    flowInk.swap();
    
    flowInk.getNewTex()->begin();
    inkFlow.update(width, height,
                   pxSize,
                   uniforms->ba1, uniforms->ba2,
                   offset,
                   velDen.getOldTex()->getTextureReference(),
                   misc.getOldTex()->getTextureReference(),
                   dist1.getOldTex()->getTextureReference(),
                   dist2.getOldTex()->getTextureReference(),
                   flowInk.getOldTex()->getTextureReference(),
                   surfInk.getOldTex()->getTextureReference());
    flowInk.getNewTex()->end();
    flowInk.swap();
}

void ofxInkSim::draw()
{
    string whatdraw = "";
    if (drawMode == INKFIX)
    {
        whatdraw = "INKFIX";
        drawXYZ(fixInk.getOldTex()->getTextureReference(), 0, 0, width, height);
    }
    else if (drawMode == INKSURF)
    {
        whatdraw = "INKSURF";
        drawXYZ(surfInk.getOldTex()->getTextureReference(), 0, 0, width, height);
    }
    else if (drawMode == INKFLOW)
    {
        whatdraw = "INKFLOW";
        drawXYZ(flowInk.getOldTex()->getTextureReference(), 0, 0, width, height);
    }
    else if (drawMode == WATERFLOW)
    {
        whatdraw = "WATERFLOW";
        drawZ(velDen.getOldTex()->getTextureReference(), 0, 0, width, height);
    }
    
    if (bDebug)
    {
        int w = width/8;
        int h = height/8;
        
        drawXYZ(surfInk.getOldTex()->getTextureReference(), 0, 0, w, h);
        drawXYZ(misc.getOldTex()->getTextureReference(), w*1, 0, w, h);
        drawXYZ(velDen.getOldTex()->getTextureReference(), w*2, 0, w, h);
        drawXYZ(flowInk.getOldTex()->getTextureReference(), w*3, 0, w, h);
        drawXYZ(fixInk.getOldTex()->getTextureReference(), w*4, 0, w, h);
        drawXYZ(dist1.getOldTex()->getTextureReference(), w*5, 0, w, h);
        drawXYZ(dist2.getOldTex()->getTextureReference(), w*6, 0, w, h);
        drawXYZ(sinkInk.getOldTex()->getTextureReference(), w*7, 0, w, h);
        
        ofDrawBitmapStringHighlight("SurfInk OLD", 0, 15);
        ofDrawBitmapStringHighlight("Misc OLD", w*1, 15);
        ofDrawBitmapStringHighlight("VelDen OLD", w*2, 15);
        ofDrawBitmapStringHighlight("FlowInk OLD", w*3, 15);
        ofDrawBitmapStringHighlight("FixInk OLD", w*4, 15);
        ofDrawBitmapStringHighlight("Dist1 OLD", w*5, 15);
        ofDrawBitmapStringHighlight("Dist2 OLD", w*6, 15);
        ofDrawBitmapStringHighlight("SinkInk OLD", w*7, 15);
        
        depositionBuffer.draw(0, h, w, h);
        grain.draw(w*1, h, w, h);
        alum.draw(w*2, h, w, h);
        pinning.draw(w*3, h, w, h);
        drawXYZ(disorder.getTextureReference(), w*4, h, w, h);
        
        drawXYZ(surfInk.getNewTex()->getTextureReference(), 0, height - h, w, h);
        drawXYZ(misc.getNewTex()->getTextureReference(), w*1, height - h, w, h);
        drawXYZ(velDen.getNewTex()->getTextureReference(), w*2, height - h, w, h);
        drawXYZ(flowInk.getNewTex()->getTextureReference(), w*3, height - h, w, h);
        drawXYZ(fixInk.getNewTex()->getTextureReference(), w*4, height - h, w, h);
        drawXYZ(dist1.getNewTex()->getTextureReference(), w*5, height - h, w, h);
        drawXYZ(dist2.getNewTex()->getTextureReference(), w*6, height - h, w, h);
        drawXYZ(sinkInk.getNewTex()->getTextureReference(), w*7, height - h, w, h);
        
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
    
    misc.clear(0);
    velDen.clear(0);
    dist1.clear(0);
    dist2.clear(0);
    surfInk.clear(0);
    flowInk.clear(0);
    fixInk.clear(0);
    sinkInk.clear(0);
}

void ofxInkSim::begin()
{
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    depositionBuffer.begin();
    ofClear(0, 0);
    ofPushStyle();
}

void ofxInkSim::end()
{
    ofPopStyle();
    depositionBuffer.end();
    
    ofDisableBlendMode();
    
    depositeOnPaperSurface();
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
    
    surfInk.getNewTex()->begin();
    ofClear(0, 0);
    addpigment.update(width, height,
                      pxSize,
                      uniforms->gamma,
                      uniforms->baseMask,
                      surfInk.getOldTex()->getTextureReference(),
                      depositionBuffer.getTextureReference(),
                      misc.getOldTex()->getTextureReference());
    surfInk.getNewTex()->end();
    surfInk.swap();
    
    misc.getNewTex()->begin();
    ofClear(0, 0);
    addwater.update(width, height,
                    pxSize,
                    uniforms->gamma,
                    uniforms->baseMask,
                    uniforms->waterAmount,
                    misc.getOldTex()->getTextureReference(),
                    depositionBuffer.getTextureReference());
    misc.getNewTex()->end();
    misc.swap();
    
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
    disorder.begin();
    GAP.update(width, height, pxSize, grain, alum, pinning);
    disorder.end();
    ofDisableBlendMode();
}

ofPtr<UniformInfos> ofxInkSim::getUniformInfo()
{
    return uniforms;
}