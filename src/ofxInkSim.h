//
//  ofxInkSim.h
//  emptyExample
//
//  Created by Akira on 5/14/15.
//
//

#ifndef __emptyExample__ofxInkSim__
#define __emptyExample__ofxInkSim__

#include "ofMain.h"
#include "FboPingPong.h"
#include "BrashIF.h"

#include "AddPigmentShader.h"
#include "AddWaterShader.h"
#include "BlockShader.h"
#include "Collide1Shader.h"
#include "Collide2Shader.h"
#include "GapShader.h"
#include "GetVelDenShader.h"
#include "GetXYZShader.h"
#include "GetZShader.h"
#include "InkFlowShader.h"
#include "InkSupplyShader.h"
#include "InkXAmtShader.h"
#include "InkXFrShader.h"
#include "InkXToShader.h"
#include "Stream1Shader.h"
#include "Stream2Shader.h"

class UniformInfos
{
public:
    
    UniformInfos()
    {
        resetToSystemDefault();
        
        parameters.setName("shader uniforms");
        parameters.add(brushsize.set("brushsize", brushsize, 1.0, 50.0));
        parameters.add(waterAmount.set("waterAmount", waterAmount, 0.00000, 2.00000)); // water
        parameters.add(gamma.set("gamma", gamma, 0.00000, 1.00000)); // pigment, water
        parameters.add(baseMask.set("baseMask", baseMask, 0.00000, 1.00000)); // pigment, water
        parameters.add(toe_p.set("toe_p", toe_p, 0.00000, 1.00000)); // block
        parameters.add(b11.set("blk1_default", b11, 0.00000, 1.00000)); // block
        parameters.add(b12.set("blk1_grain", b12, 0.00000, 1.00000)); // block
        parameters.add(b13.set("blk1_alum", b13, 0.00000, 1.00000)); // block
        parameters.add(b21.set("blk2_glue", b21, 0.00000, 1.00000)); // block
        parameters.add(b22.set("blk2_fixblk", b22, 0.00000, 1.00000)); // block
        parameters.add(p1.set("pinw_default", p1, 0.00000, 1.00000)); // block
        parameters.add(p2.set("pinw_fixblk", p2, 0.00000, 1.00000)); // block
        parameters.add(p3.set("pinw_pindisorder(g,p,step(0,toe,glue))", p3, 0.00000, 1.00000));
        parameters.add(omega.set("omega", omega, 0.00000, 1.00000)); // block, collide1, collide2
        parameters.add(advect_p.set("advect_p", advect_p, 0.00000, 1.00000)); // block, collide1, collide2
        parameters.add(evapor.set("evapor", evapor, 0.00000, 1.00000)); // velden, stream1, stream2
        parameters.add(evapor_b.set("evapor_b", evapor_b, 0.00000, 1.00000)); // stream1, stream2
        parameters.add(ba1.set("blka_lowest_hindrance_rate", ba1, 0.00000, 1.00000)); // inkflow
        parameters.add(ba2.set("blka_blocking_advection_when_flow_speed_low", ba2, 0.00000, 1.00000)); // inkflow
        parameters.add(f1.set("fixrate_base_fix_factor", f1, 0.00000, 1.00000)); // inkxamt
        parameters.add(f2.set("fixrate_modulating_dryness_glue1", f2, 0.00000, 1.00000));  // inkxamt
        parameters.add(f3.set("fixrate_modulating_dryness_glue2", f3, 0.00000, 1.00000));  // inkxamt
    }
    
    void resetToSystemDefault()
    {
        brushsize = 7.086956501;
        baseMask = 0.037267081;
        gamma = 0.037267081;
        omega = 0.968944073;
        advect_p = 0.100000001;
        evapor_b = 0.000010000;
        evapor = 0.000500000;
        b11 = 0.009316770;
        b12 = 0.391304344;
        b13 = 0.009316770;
        b21 = 0.123152710;
        b22 = 0.307453424;
        p1 = 0.000000000;
        p2 = 0.300000012;
        p3 = 0.200000003;
        ba1 = 0.000040994;
        ba2 = 0.000043168;
        f1 = 0.010000000;
        f2 = 0.090000004;
        f3 = 0.090000004;
        toe_p = 0.100000001;
        waterAmount = 1.000000000;
        wf_mul = 1.0f;
    }
    
    ofParameterGroup parameters;
    ofParameter<float> brushsize;
    ofParameter<float> baseMask;
    ofParameter<float> gamma;
    ofParameter<float> omega;
    ofParameter<float> advect_p;
    ofParameter<float> evapor_b;
    ofParameter<float> evapor;
    ofParameter<float> b11;
    ofParameter<float> b12;
    ofParameter<float> b13;
    ofParameter<float> b21;
    ofParameter<float> b22;
    ofParameter<float> p1;
    ofParameter<float> p2;
    ofParameter<float> p3;
    ofParameter<float> ba1;
    ofParameter<float> ba2;
    ofParameter<float> f1;
    ofParameter<float> f2;
    ofParameter<float> f3;
    ofParameter<float> toe_p;
    ofParameter<float> waterAmount;
    ofParameter<float> wf_mul;
};

struct StrokeInfo
{
    ofPtr<BrashIF> brush;
    ofPoint pos;
    ofColor col;
    float brushSize;
};

class ofxInkSim
{
public:
    
    enum DRAWMODE
    {
        INKFIX,
        INKSURF,
        INKFLOW,
        WATERFLOW
    };
    DRAWMODE drawMode;
    
    ofxInkSim() { uniforms = ofPtr<UniformInfos>(new UniformInfos); }
    void setup(int width = ofGetWidth(), int height = ofGetHeight(),
               string grainPath = "tex/grain.jpg", string alumPath = "tex/alum3.jpg", string pinningPath = "tex/pinning.jpg");
    void update(bool bEvaporToDisapper = false);
    void draw();
    void stroke(vector<StrokeInfo> sis);
    void stroke(BrashIF* brush, int x, int y, ofColor brushCol);
    void stroke(BrashIF* brush, int x, int y, ofColor brushCol, float brushSize);
    void stopStroke(vector<StrokeInfo> sis);
    void stopStroke(BrashIF* brush);
    void begin();
    void end();
    void clear();
    void setDrawMode(DRAWMODE drawMode);
    void toggleDebug();
    ofPtr<UniformInfos> getUniformInfo();
    
protected:
    
    void depositeOnPaperSurface();
    void drawXYZ(ofTexture& texRef, float x, float y, float w, float h);
    void drawZ(ofTexture& texRef, float x, float y, float w, float h);
    void drawPlane();
    void fillDisorderBuffer();
    
    int width;
    int height;
    
    ofVec2f pxSize;
    ofVec2f offset;
    
    GapShader GAP;
    AddPigmentShader addpigment;
    AddWaterShader addwater;
    BlockShader block;
    Collide1Shader collide1;
    Collide2Shader collide2;
    Stream1Shader stream1;
    Stream2Shader stream2;
    GetVelDenShader getVelDen;
    InkSupplyShader inkSupply;
    InkXAmtShader inkXAmt;
    InkXToShader inkXTo;
    InkXFrShader inkXFr;
    InkFlowShader inkFlow;
    GetXYZShader getXYZ;
    GetZShader getZ;
    ofPtr<UniformInfos> uniforms;
    
    ofFbo screen;
    ofFbo depositionBuffer;
    ofTexture grain;
    ofTexture alum;
    ofTexture pinning;
    ofFbo disorder;
    
    FboPingPong misc;
    FboPingPong velDen;
    FboPingPong dist1;
    FboPingPong dist2;
    FboPingPong surfInk;
    FboPingPong flowInk;
    FboPingPong fixInk;
    FboPingPong sinkInk;
    
    ofStyle style;
    
    // debug
    bool bDebug;
};

// https://www.google.co.jp/search?q=hue+angle&espv=2&biw=1029&bih=1101&tbm=isch&imgil=9QN3BC-gOQeVoM%253A%253BP5j4a65YEiF6iM%253Bhttp%25253A%25252F%25252Fflylib.com%25252Fbooks%25252Fen%25252F2.816.1.70%25252F1%25252F&source=iu&pf=m&fir=9QN3BC-gOQeVoM%253A%252CP5j4a65YEiF6iM%252C_&usg=__WQVpLRXWNy_h_WlylScvAJ6VsC0%3D&ved=0CCsQyjc&ei=g9eEVZeBPOXQmwXE9YGwCA#imgrc=9QN3BC-gOQeVoM%253A%3BP5j4a65YEiF6iM%3Bhttp%253A%252F%252Fflylib.com%252Fbooks%252F2%252F816%252F1%252Fhtml%252F2%252Ffiles%252Ffig11_14.jpeg%3Bhttp%253A%252F%252Fflylib.com%252Fbooks%252Fen%252F2.816.1.70%252F1%252F%3B350%3B323&usg=__WQVpLRXWNy_h_WlylScvAJ6VsC0%3D
// depth = 1 - 255
static ofColor getInkColor(float hueAngle, float sat = 255, float depth = 255)
{
    ofColor c = ofColor::red;
    float hue = hueAngle;
    hue += 180;
    if (hue > 360)
        hue -= 360;
    c.setHueAngle(hue);
    c.setSaturation(sat);
    c.setBrightness(depth);
    return c;
}

#endif /* defined(__emptyExample__ofxInkSim__) */
