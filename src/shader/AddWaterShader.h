//
//  AddWaterShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_AddWaterShader_h
#define ofxInkSimExample_AddWaterShader_h

#include "ofMain.h"
#include "InkShader.h"

class AddWaterShader : public InkShader
{
public:
    
    AddWaterShader()
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
                float waterAmount,
                ofTexture& Misc,
                ofTexture& depositionBuffer)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniform1f("gamma", gamma);
        shader.setUniform1f("baseMask", baseMask);
        shader.setUniform1f("waterAmount", waterAmount);
        shader.setUniformTexture("Misc", Misc, 0);
        shader.setUniformTexture("WaterSurface", depositionBuffer, 1);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D Misc;
                                 uniform sampler2D WaterSurface;
                                 
                                 uniform float gamma;
                                 uniform float baseMask;
                                 uniform float waterAmount;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 mi = texture2D(Misc, Tex0);
                                     vec4 wa = texture2D(WaterSurface, Tex0);
                                     float DepMask = max(1.0 - mi.z / gamma, baseMask);
                                     
                                     float temp_waterAmount = 0.0;
                                     
                                     if (wa.w != 0.0)
                                     {
                                         temp_waterAmount = waterAmount;
                                     }
                                     
                                     gl_FragColor = vec4(mi.xyz, mi.w + clamp(temp_waterAmount, 0.0, DepMask));
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
                                 uniform sampler2D Misc;
                                 uniform sampler2D WaterSurface;
                                 
                                 uniform float gamma;
                                 uniform float baseMask;
                                 uniform float waterAmount;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 mi = texture(Misc, Tex0);
                                     vec4 wa = texture(WaterSurface, Tex0);
                                     float DepMask = max(1.0 - mi.z / gamma, baseMask);
                                     
                                     float temp_waterAmount = 0.0;
                                     
                                     if (wa.w != 0.0)
                                     {
                                         temp_waterAmount = waterAmount;
                                     }
                                     
                                     fragColor = vec4(mi.xyz, mi.w + clamp(temp_waterAmount, 0.0, DepMask));
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
