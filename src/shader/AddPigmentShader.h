//
//  AddPigmentShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_AddPigmentShader_h
#define ofxInkSimExample_AddPigmentShader_h

#include "ofMain.h"
#include "InkShader.h"

class AddPigmentShader : public InkShader
{
public:
    
    AddPigmentShader()
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
                float gamma,
                float baseMask,
                ofTexture& SurfInk,
                ofTexture& depositionBuffer,
                ofTexture& Misc)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniform1f("gamma", gamma);
        shader.setUniform1f("baseMask", baseMask);
        shader.setUniformTexture("SurfInk", SurfInk, 0);
        shader.setUniformTexture("WaterSurface", depositionBuffer, 1);
        shader.setUniformTexture("Misc", Misc, 2);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D SurfInk;
                                 uniform sampler2D WaterSurface;
                                 uniform sampler2D Misc;
                                 uniform sampler2D test;
                                 
                                 uniform float gamma;
                                 uniform float baseMask;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 is = texture2D(SurfInk, Tex0);
                                     vec4 wa = texture2D(WaterSurface, Tex0);
                                     vec4 mi = texture2D(Misc, Tex0);
                                     
                                     float DepMask = max(1.0 - mi.z / gamma, baseMask);
                                     
                                     float re = is.x + clamp(wa.x, 0.0, DepMask);
                                     float gr = is.y + clamp(wa.y, 0.0, DepMask);
                                     float bl = is.z + clamp(wa.z, 0.0, DepMask);
                                     
                                     gl_FragColor = vec4(re, gr, bl, is.w);
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
                                 uniform sampler2D SurfInk;
                                 uniform sampler2D WaterSurface;
                                 uniform sampler2D Misc;
                                 
                                 uniform float gamma;
                                 uniform float baseMask;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 is = texture(SurfInk, Tex0);
                                     vec4 wa = texture(WaterSurface, Tex0);
                                     vec4 mi = texture(Misc, Tex0);
                                     
                                     float DepMask = max(1.0 - mi.z / gamma, baseMask);
                                     
                                     float re = is.x + clamp(wa.x, 0.0, DepMask);
                                     float gr = is.y + clamp(wa.y, 0.0, DepMask); 
                                     float bl = is.z + clamp(wa.z, 0.0, DepMask);
                                     
                                     fragColor = vec4(re, gr, bl, is.w);
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
