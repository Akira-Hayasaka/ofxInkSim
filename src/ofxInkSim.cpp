//
//  ofxInkSim.cpp
//  emptyExample
//
//  Created by Akira on 5/14/15.
//
//

#include "ofxInkSim.h"

void ofxInkSim::setup(BrashIF* brush, int width, int height)
{
    this->brush = brush;
    this->width = width;
    this->height = height;
    
    ofDisableArbTex();
    
    pxSize = ofVec2f::one() / ofVec2f(width, height);
    offset = pxSize;
    
    quad.getVertices().resize(4);
    quad.getTexCoords().resize(4);
    quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    
    if (ofGetGLProgrammableRenderer())
    {
        GAP.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/gap.frag");
        addpigment.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/addpigment.frag");
        addwater.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/addwater.frag");
        block.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/block.frag");
        collide1.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/collide1.frag");
        collide2.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/collide2.frag");
        stream1.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/stream1.frag");
        stream2.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/stream2.frag");
        GetVelDen.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/getvelden.frag");
        InkSupply.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/inksupply.frag");
        InkXAmt.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/inkxamt.frag");
        InkXTo.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/inkxto.frag");
        InkXFr.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/inkxfr.frag");
        InkFlow.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/inkflow.frag");
        getXYZ.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/getXYZ.frag");
        getZ.load("shaders/shadersGL3/simpleTex.vert", "shaders/shadersGL3/getz.frag");
    }
    else
    {
        GAP.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/gap.frag");
        addpigment.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/addpigment.frag");
        addwater.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/addwater.frag");
        block.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/block.frag");
        collide1.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/collide1.frag");
        collide2.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/collide2.frag");
        stream1.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/stream1.frag");
        stream2.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/stream2.frag");
        GetVelDen.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/getvelden.frag");
        InkSupply.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/inksupply.frag");
        InkXAmt.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/inkxamt.frag");
        InkXTo.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/inkxto.frag");
        InkXFr.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/inkxfr.frag");
        InkFlow.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/inkflow.frag");
        getXYZ.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/getXYZ.frag");
        getZ.load("shaders/shadersGL2/simpleTex.vert", "shaders/shadersGL2/getz.frag");
    }
    
    ofLoadImage(Grain, "tex/grain.jpg");
    ofLoadImage(Alum, "tex/alum3.jpg");
    ofLoadImage(Pinning, "tex/pinning.jpg");
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
    block.begin();
    block.setUniform2fv("pxSize", pxSize.getPtr());
    block.setUniform1f("A0", 0.4444444f);
    block.setUniform1f("advect_p", uniforms->advect_p);
    block.setUniform3f("Blk_1", uniforms->b11, uniforms->b12, uniforms->b13);
    block.setUniform2f("Blk_2", uniforms->b21, uniforms->b22);
    block.setUniform3f("Pin_w", uniforms->p1, uniforms->p2, uniforms->p3);
    block.setUniform1f("toe_p", uniforms->toe_p);
    block.setUniform1f("Omega", uniforms->omega);
    block.setUniform1f("Corn_mul", pow(2.0f, 0.5f));
    block.setUniform2fv("offset", offset.getPtr());
    block.setUniformTexture("MiscMap", Misc.getOldTex()->getTextureReference(), 0);
    block.setUniformTexture("VelDenMap", VelDen.getOldTex()->getTextureReference(), 1);
    block.setUniformTexture("FlowInkMap", FlowInk.getOldTex()->getTextureReference(), 2);
    block.setUniformTexture("FixInkMap", FixInk.getOldTex()->getTextureReference(), 3);
    block.setUniformTexture("DisorderMap", Disorder.getTextureReference(), 4);
    drawPlane(width, height);
    block.end();
    Misc.getNewTex()->end();
    Misc.swap();
    
    Dist1.getNewTex()->begin();
    collide1.begin();
    collide1.setUniform2fv("pxSize", pxSize.getPtr());
    collide1.setUniform1f("A", 0.1111111f);
    collide1.setUniform1f("B", 0.3333333f);
    collide1.setUniform1f("C", 0.5f);
    collide1.setUniform1f("D", 0.1666667f);
    collide1.setUniform1f("advect_p", uniforms->advect_p);
    collide1.setUniform1f("Omega", uniforms->omega);
    collide1.setUniformTexture("VelDenMap", VelDen.getOldTex()->getTextureReference(), 0);
    collide1.setUniformTexture("Dist1Map", Dist1.getOldTex()->getTextureReference(), 1);
    collide1.setUniformTexture("InkMap", FlowInk.getOldTex()->getTextureReference(), 2);
    drawPlane(width, height);
    collide1.end();
    Dist1.getNewTex()->end();
    Dist1.swap();
    
    Dist2.getNewTex()->begin();
    collide2.begin();
    collide2.setUniform2fv("pxSize", pxSize.getPtr());
    collide2.setUniform1f("A", 0.02777778f);
    collide2.setUniform1f("B", 0.08333334f);
    collide2.setUniform1f("C", 0.125f);
    collide2.setUniform1f("D", 0.04166667f);
    collide2.setUniform1f("advect_p", uniforms->advect_p);
    collide2.setUniform1f("Omega", uniforms->omega);
    collide2.setUniformTexture("VelDenMap", VelDen.getOldTex()->getTextureReference(), 0);
    collide2.setUniformTexture("Dist2Map", Dist2.getOldTex()->getTextureReference(), 1);
    collide2.setUniformTexture("InkMap", FlowInk.getOldTex()->getTextureReference(), 2);
    drawPlane(width, height);
    collide2.end();
    Dist2.getNewTex()->end();
    Dist2.swap();
    
    Dist1.getNewTex()->begin();
    stream1.begin();
    stream1.setUniform2fv("pxSize", pxSize.getPtr());
    stream1.setUniform1f("Evapor_b", uniforms->evapor_b);
    stream1.setUniform2fv("offset", offset.getPtr());
    stream1.setUniformTexture("MiscMap", Misc.getOldTex()->getTextureReference(), 0);
    stream1.setUniformTexture("Dist1Map", Dist1.getOldTex()->getTextureReference(), 1);
    drawPlane(width, height);
    stream1.end();
    Dist1.getNewTex()->end();
    Dist1.swap();
    
    Dist2.getNewTex()->begin();
    stream2.begin();
    stream2.setUniform2fv("pxSize", pxSize.getPtr());
    stream2.setUniform1f("Evapor_b", uniforms->evapor_b);
    stream2.setUniform2fv("offset", offset.getPtr());
    stream2.setUniformTexture("MiscMap", Misc.getOldTex()->getTextureReference(), 0);
    stream2.setUniformTexture("Dist2Map", Dist2.getOldTex()->getTextureReference(), 1);
    drawPlane(width, height);
    stream2.end();
    Dist2.getNewTex()->end();
    Dist2.swap();
    
    VelDen.getNewTex()->begin();
    GetVelDen.begin();
    GetVelDen.setUniform2fv("pxSize", pxSize.getPtr());
    GetVelDen.setUniform1f("wf_mul", uniforms->wf_mul);
    GetVelDen.setUniform1f("Evapor", uniforms->evapor);
    GetVelDen.setUniformTexture("MiscMap", Misc.getOldTex()->getTextureReference(), 0);
    GetVelDen.setUniformTexture("Dist1Map", Dist1.getOldTex()->getTextureReference(), 1);
    GetVelDen.setUniformTexture("Dist2Map", Dist2.getOldTex()->getTextureReference(), 2);
    GetVelDen.setUniformTexture("VelDenMap", VelDen.getOldTex()->getTextureReference(), 3);
    drawPlane(width, height);
    GetVelDen.end();
    VelDen.getNewTex()->end();
    VelDen.swap();
    
    SurfInk.getNewTex()->begin();
    InkSupply.begin();
    InkSupply.setUniform2fv("pxSize", pxSize.getPtr());
    InkSupply.setUniformTexture("VelDenMap", VelDen.getOldTex()->getTextureReference(), 0);
    InkSupply.setUniformTexture("SurfInkMap", SurfInk.getOldTex()->getTextureReference(), 1);
    InkSupply.setUniformTexture("MiscMap", Misc.getOldTex()->getTextureReference(), 2);
    drawPlane(width, height);
    InkSupply.end();
    SurfInk.getNewTex()->end();
    SurfInk.swap();
    
    SinkInk.getNewTex()->begin();
    InkXAmt.begin();
    InkXAmt.setUniform2fv("pxSize", pxSize.getPtr());
    InkXAmt.setUniform3f("FixRate", uniforms->f1, uniforms->f2, uniforms->f3);
    InkXAmt.setUniformTexture("VelDenMap", VelDen.getOldTex()->getTextureReference(), 0);
    InkXAmt.setUniformTexture("MiscMap", Misc.getOldTex()->getTextureReference(), 1);
    InkXAmt.setUniformTexture("FlowInkMap", FlowInk.getOldTex()->getTextureReference(), 2);
    InkXAmt.setUniformTexture("FixInkMap", FixInk.getOldTex()->getTextureReference(), 3);
    drawPlane(width, height);
    InkXAmt.end();
    SinkInk.getNewTex()->end();
    SinkInk.swap();
    
    FixInk.getNewTex()->begin();
    InkXTo.begin();
    InkXTo.setUniform2fv("pxSize", pxSize.getPtr());
    InkXTo.setUniformTexture("FixInkMap", FixInk.getOldTex()->getTextureReference(), 0);
    InkXTo.setUniformTexture("SinkInkMap", SinkInk.getOldTex()->getTextureReference(), 1);
    drawPlane(width, height);
    InkXTo.end();
    FixInk.getNewTex()->end();
    FixInk.swap();
    
    FlowInk.getNewTex()->begin();
    InkXFr.begin();
    InkXFr.setUniform2fv("pxSize", pxSize.getPtr());
    InkXFr.setUniformTexture("FlowInkMap", FlowInk.getOldTex()->getTextureReference(), 0);
    InkXFr.setUniformTexture("SinkInkMap", SinkInk.getOldTex()->getTextureReference(), 1);
    drawPlane(width, height);
    InkXFr.end();
    FlowInk.getNewTex()->end();
    FlowInk.swap();
    
    FlowInk.getNewTex()->begin();
    InkFlow.begin();
    InkFlow.setUniform2fv("pxSize", pxSize.getPtr());
    InkFlow.setUniform2f("Blk_a", uniforms->ba1, uniforms->ba2);
    InkFlow.setUniform2fv("offset", offset.getPtr());
    InkFlow.setUniformTexture("VelDenMap", VelDen.getOldTex()->getTextureReference(), 0);
    InkFlow.setUniformTexture("MiscMap", Misc.getOldTex()->getTextureReference(), 1);
    InkFlow.setUniformTexture("Dist1Map", Dist1.getOldTex()->getTextureReference(), 2);
    InkFlow.setUniformTexture("Dist2Map", Dist2.getOldTex()->getTextureReference(), 3);
    InkFlow.setUniformTexture("FlowInkMap", FlowInk.getOldTex()->getTextureReference(), 4);
    InkFlow.setUniformTexture("SurfInkMap", SurfInk.getOldTex()->getTextureReference(), 5);
    drawPlane(width, height);
    InkFlow.end();
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

void ofxInkSim::stroke(int x, int y, ofColor brushCol)
{
    stroke(x, y, brushCol, uniforms->brushsize);
}

void ofxInkSim::stroke(int x, int y, ofColor brushCol, float brushSize)
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

void ofxInkSim::stopStroke()
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
    addpigment.begin();
    addpigment.setUniform2fv("pxSize", pxSize.getPtr());
    addpigment.setUniform1f("gamma", uniforms->gamma);
    addpigment.setUniform1f("baseMask", uniforms->baseMask);
    addpigment.setUniformTexture("SurfInk", SurfInk.getOldTex()->getTextureReference(), 0);
    addpigment.setUniformTexture("WaterSurface", depositionBuffer.getTextureReference(), 1);
    addpigment.setUniformTexture("Misc", Misc.getOldTex()->getTextureReference(), 2);
    drawPlane(width, height);
    addpigment.end();
    SurfInk.getNewTex()->end();
    SurfInk.swap();
    
    Misc.getNewTex()->begin();
    ofClear(0, 0);
    addwater.begin();
    addwater.setUniform2fv("pxSize", pxSize.getPtr());
    addwater.setUniform1f("gamma", uniforms->gamma);
    addwater.setUniform1f("baseMask", uniforms->baseMask);
    addwater.setUniform1f("waterAmount", uniforms->waterAmount);
    addwater.setUniformTexture("Misc", Misc.getOldTex()->getTextureReference(), 0);
    addwater.setUniformTexture("WaterSurface", depositionBuffer.getTextureReference(), 1);
    drawPlane(width, height);
    addwater.end();
    Misc.getNewTex()->end();
    Misc.swap();
    
    ofDisableBlendMode();
}

void ofxInkSim::drawXYZ(ofTexture& texRef, float x, float y, float w, float h)
{
    screen.begin();
    ofClear(0, 0);
    getXYZ.begin();
    getXYZ.setUniform2fv("pxSize", pxSize.getPtr());
    getXYZ.setUniformTexture("src", texRef, 0);
    drawPlane(width, height);
    getXYZ.end();
    screen.end();
    screen.draw(x, y, w, h);
}

void ofxInkSim::drawZ(ofTexture& texRef, float x, float y, float w, float h)
{
    screen.begin();
    ofClear(0, 0);
    getZ.begin();
    getZ.setUniform2fv("pxSize", pxSize.getPtr());
    getZ.setUniformTexture("src", texRef, 0);
    drawPlane(width, height);
    getZ.end();
    screen.end();
    screen.draw(x, y, w, h);
}

void ofxInkSim::fillDisorderBuffer()
{
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    Disorder.begin();
    GAP.begin();
    GAP.setUniform2fv("pxSize", pxSize.getPtr());
    GAP.setUniformTexture("Grain", Grain, 0);
    GAP.setUniformTexture("Alum", Alum, 1);
    GAP.setUniformTexture("Pinning", Pinning, 2);
    drawPlane(width, height);
    GAP.end();
    Disorder.end();
    ofDisableBlendMode();
}

ofPtr<UniformInfos> ofxInkSim::getUniformInfo()
{
    return uniforms;
}