//
//  GetVelDenShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_GetVelDenShader_h
#define ofxInkSimExample_GetVelDenShader_h

#include "ofMain.h"
#include "InkShader.h"

class GetVelDenShader : public InkShader
{
public:
    
    GetVelDenShader()
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
                float wf_mul,
                float evapor,
                ofTexture& Misc,
                ofTexture& Dist1,
                ofTexture& Dist2,
                ofTexture& VelDen)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniform1f("wf_mul", wf_mul);
        shader.setUniform1f("Evapor", evapor);
        shader.setUniformTexture("MiscMap", Misc, 0);
        shader.setUniformTexture("Dist1Map", Dist1, 1);
        shader.setUniformTexture("Dist2Map", Dist2, 2);
        shader.setUniformTexture("VelDenMap", VelDen, 3);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform float wf_mul;
                                 uniform float Evapor;
                                 
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D Dist1Map;
                                 uniform sampler2D Dist2Map;
                                 uniform sampler2D VelDenMap;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 Misc0 = texture2D(MiscMap, Tex0);
                                     float f0 = Misc0.y;
                                     float ws = Misc0.w;
                                     
                                     vec4 Dist1 = texture2D(Dist1Map, Tex0);
                                     vec4 Dist2 = texture2D(Dist2Map, Tex0);
                                     vec2 v;
                                     v.x = (Dist1.y + Dist2.x + Dist2.y) - (Dist1.z + Dist2.z + Dist2.w);
                                     v.y = (Dist1.w + Dist2.y + Dist2.w) - (Dist1.x + Dist2.x + Dist2.z);
                                     
                                     vec4 tmp = Dist1 + Dist2;
                                     float wf = f0 + tmp.x + tmp.y + tmp.z + tmp.w;
                                     
                                     wf *= wf_mul;
                                     wf = max(wf - Evapor, 0.0);
                                     float seep = clamp(ws / 15.0, 0.0, max(1.0 - wf, 0.0));
                                     wf += seep;
                                     
                                     gl_FragColor = vec4(v.x, v.y, wf, seep);
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
                                 uniform float wf_mul;
                                 uniform float Evapor;
                                 
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D Dist1Map;
                                 uniform sampler2D Dist2Map;
                                 uniform sampler2D VelDenMap;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 Misc0 = texture(MiscMap, Tex0);
                                     float f0 = Misc0.y;
                                     float ws = Misc0.w;
                                     
                                     vec4 Dist1 = texture(Dist1Map, Tex0);
                                     vec4 Dist2 = texture(Dist2Map, Tex0);
                                     vec2 v;
                                     v.x = (Dist1.y + Dist2.x + Dist2.y) - (Dist1.z + Dist2.z + Dist2.w);
                                     v.y = (Dist1.w + Dist2.y + Dist2.w) - (Dist1.x + Dist2.x + Dist2.z);
                                     
                                     vec4 tmp = Dist1 + Dist2;
                                     float wf = f0 + tmp.x + tmp.y + tmp.z + tmp.w;
                                     
                                     wf *= wf_mul;
                                     wf = max(wf - Evapor, 0.0);
                                     float seep = clamp(ws / 15.0, 0.0, max(1.0 - wf, 0.0));
                                     wf += seep;
                                     
                                     fragColor = vec4(v.x, v.y, wf, seep);
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
