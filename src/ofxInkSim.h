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
    
    void setup(BrashIF* brush);
    void update();
    void draw();
    void stroke(int x, int y, ofColor brushCol);
    void stopStroke();
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
    
    BrashIF* brush;
    
    ofVec2f pxSize;
    ofVec2f offset;
    
    ofShader GAP;
    ofShader addpigment;
    ofShader addwater;
    ofShader block;
    ofShader collide1;
    ofShader collide2;
    ofShader stream1;
    ofShader stream2;
    ofShader GetVelDen;
    ofShader InkSupply;
    ofShader InkXAmt;
    ofShader InkXTo;
    ofShader InkXFr;
    ofShader InkFlow;
    ofShader getXYZ;
    ofShader getZ;
    ofPtr<UniformInfos> uniforms;
    
    ofPlanePrimitive plane;
    ofFbo screen;
    ofFbo depositionBuffer;
    ofTexture Grain;
    ofTexture Alum;
    ofTexture Pinning;
    ofFbo Disorder;
    
    FboPingPong Misc;
    FboPingPong VelDen;
    FboPingPong Dist1;
    FboPingPong Dist2;
    FboPingPong SurfInk;
    FboPingPong FlowInk;
    FboPingPong FixInk;
    FboPingPong SinkInk;
    
    // debug
    bool bDebug;
};

#endif /* defined(__emptyExample__ofxInkSim__) */
