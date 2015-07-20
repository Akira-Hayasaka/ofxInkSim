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
    }
    
    void resetToSystemDefault()
    {
        brushsize = 10.0f;
        baseMask = 0.4f;
        gamma = 0.8f;
        omega = 0.5f;
        advect_p = 0.1f;
        evapor_b = 1E-05f;
        evapor = 0.0005f;
        b11 = 0.0f;
        b12 = 0.1f;
        b13 = 0.05f;
        b21 = 0.0f;
        b22 = 0.0f;
        p1 = 0.0f;
        p2 = 0.3f;
        p3 = 0.2f;
        ba1 = 0.0f;
        ba2 = 1E-09f;
        f1 = 0.001f;
        f2 = 0.09f;
        //    f3 = 1E-05f;
        f3 = 0.09f;
        toe_p = 0.1f;
        waterAmount = 1.0f;
        wf_mul = 1.0f;
        brushsize = 10.0f;
    }
    
    float brushsize;
    float baseMask;
    float gamma;
    float omega;
    float advect_p;
    float evapor_b;
    float evapor;
    float b11;
    float b12;
    float b13;
    float b21;
    float b22;
    float p1;
    float p2;
    float p3;
    float ba1;
    float ba2;
    float f1;
    float f2;
    float f3;
    float toe_p;
    float waterAmount;
    float wf_mul;
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
    void update();
    void draw();
    void stroke(vector<StrokeInfo> sis);
    void stroke(BrashIF* brush, int x, int y, ofColor brushCol);
    void stroke(BrashIF* brush, int x, int y, ofColor brushCol, float brushSize);
    void stopStroke(vector<StrokeInfo> sis);
    void stopStroke(BrashIF* brush);
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
