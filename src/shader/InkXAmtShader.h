//
//  InkXAmtShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_InkXAmtShader_h
#define ofxInkSimExample_InkXAmtShader_h

#include "ofMain.h"
#include "InkShader.h"

class InkXAmtShader : public InkShader
{
public:
    
    InkXAmtShader()
    {
        if (isProgrammableRenderer)
        {
            glThree();
        }
        else
        {
            glTwo();
        }
    }
    
    void update(int w, int h,
                ofVec2f pxSize,
                float f1, float f2, float f3,
                ofTexture& VelDen,
                ofTexture& Misc,
                ofTexture& FlowInk,
                ofTexture& FixInk)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniform3f("FixRate", f1, f2, f3);
        shader.setUniformTexture("VelDenMap", VelDen, 0);
        shader.setUniformTexture("MiscMap", Misc, 1);
        shader.setUniformTexture("FlowInkMap", FlowInk, 2);
        shader.setUniformTexture("FixInkMap", FixInk, 3);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D VelDenMap;
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D FlowInkMap;
                                 uniform sampler2D FixInkMap;
                                 
                                 uniform vec3 FixRate;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     float lwf = texture2D(MiscMap, Tex0).z;
                                     float wf = texture2D(VelDenMap, Tex0).z;
                                     vec4 if0 = texture2D(FlowInkMap, Tex0);
                                     
                                     float wLoss = max(lwf - wf, 0.0);
                                     float FixFactor;
                                     
                                     if (wLoss > 0.0)
                                         FixFactor = wLoss / lwf;
                                     else
                                         FixFactor = 0.0;
                                     
                                     float mu_star = clamp(FixRate.y + FixRate.z * if0.w, 0.0, 1.0);
                                     FixFactor = max(FixFactor * (1.0 - smoothstep(0.0, mu_star, wf)), FixRate.x);
                                     
                                     vec4 sink = if0 * FixFactor;
                                     
                                     gl_FragColor = sink;
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
    
    void glThree()
    {
        fragmentShader = GLSL150(
                                 uniform sampler2D VelDenMap;
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D FlowInkMap;
                                 uniform sampler2D FixInkMap;
                                 
                                 uniform vec3 FixRate;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     float lwf = texture(MiscMap, Tex0).z;
                                     float wf = texture(VelDenMap, Tex0).z;
                                     vec4 if0 = texture(FlowInkMap, Tex0);
                                     
                                     float wLoss = max(lwf - wf, 0.0);
                                     float FixFactor;
                                     
                                     if (wLoss > 0.0)
                                         FixFactor = wLoss / lwf;
                                     else
                                         FixFactor = 0.0;
                                     
                                     float mu_star = clamp(FixRate.y + FixRate.z * if0.w, 0.0, 1.0);
                                     FixFactor = max(FixFactor * (1.0 - smoothstep(0.0, mu_star, wf)), FixRate.x);
                                     
                                     vec4 sink = if0 * FixFactor;
                                     
                                     fragColor = sink;
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
