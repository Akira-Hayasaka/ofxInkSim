//
//  Stream2Shader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_Stream2Shader_h
#define ofxInkSimExample_Stream2Shader_h

#include "ofMain.h"
#include "InkShader.h"

class Stream2Shader : public InkShader
{
public:
    
    Stream2Shader()
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
                float evapor_b,
                ofVec2f offset,
                ofTexture& misc,
                ofTexture& dist2)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniform1f("Evapor_b", evapor_b);
        shader.setUniform2fv("offset", offset.getPtr());
        shader.setUniformTexture("MiscMap", misc, 0);
        shader.setUniformTexture("Dist2Map", dist2, 1);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D Dist2Map;
                                 
                                 uniform float Evapor_b;
                                 uniform vec2 offset;
                                 
                                 void main(void)
                                 {
                                     float dx = offset.s;
                                     float dy = offset.t;
                                     
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 TexN_NE = vec4(Tex0.x,      Tex0.y - dy, Tex0.x + dx, Tex0.y - dy);
                                     vec4 TexE_SE = vec4(Tex0.x + dx, Tex0.y,      Tex0.x + dx, Tex0.y + dy);
                                     vec4 TexW_NW = vec4(Tex0.x - dx, Tex0.y,      Tex0.x - dx, Tex0.y - dy);
                                     vec4 TexS_SW = vec4(Tex0.x,      Tex0.y + dy, Tex0.x - dx, Tex0.y + dy);
                                     
                                     float b0 = texture2D(MiscMap, Tex0).x;
                                     float bNE = texture2D(MiscMap, TexN_NE.zw).x;
                                     float bSE = texture2D(MiscMap, TexE_SE.zw).x;
                                     float bNW = texture2D(MiscMap, TexW_NW.zw).x;
                                     float bSW = texture2D(MiscMap, TexS_SW.zw).x;
                                     
                                     vec4 b = vec4(bSW, bNW, bSE, bNE);
                                     b = (b + b0) / 2.0;
                                     
                                     vec4 pinned = vec4(b.x > 1.0, b.y > 1.0, b.z > 1.0, b.w > 1.0);
                                     b = min(b, 1.0);
                                     vec4 f_Out;
                                     vec4 f_In;
                                     
                                     f_Out = texture2D(Dist2Map, Tex0);
                                     f_In.x = texture2D(Dist2Map, TexS_SW.zw).x;
                                     f_In.y = texture2D(Dist2Map, TexW_NW.zw).y;
                                     f_In.z = texture2D(Dist2Map, TexE_SE.zw).z;
                                     f_In.w = texture2D(Dist2Map, TexN_NE.zw).w;
                                     
                                     vec4 OUT = mix(f_In, f_Out.wzyx, b);
                                     
                                     gl_FragColor = max(OUT - pinned * Evapor_b, 0.0);
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
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D Dist2Map;
                                 
                                 uniform float Evapor_b;
                                 uniform vec2 offset;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     float dx = offset.s;
                                     float dy = offset.t;
                                     
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 TexN_NE = vec4(Tex0.x,      Tex0.y - dy, Tex0.x + dx, Tex0.y - dy);
                                     vec4 TexE_SE = vec4(Tex0.x + dx, Tex0.y,      Tex0.x + dx, Tex0.y + dy);
                                     vec4 TexW_NW = vec4(Tex0.x - dx, Tex0.y,      Tex0.x - dx, Tex0.y - dy);
                                     vec4 TexS_SW = vec4(Tex0.x,      Tex0.y + dy, Tex0.x - dx, Tex0.y + dy);
                                     
                                     float b0 = texture(MiscMap, Tex0).x;
                                     float bNE = texture(MiscMap, TexN_NE.zw).x;
                                     float bSE = texture(MiscMap, TexE_SE.zw).x;
                                     float bNW = texture(MiscMap, TexW_NW.zw).x;
                                     float bSW = texture(MiscMap, TexS_SW.zw).x;
                                     
                                     vec4 b = vec4(bSW, bNW, bSE, bNE);
                                     b = (b + b0) / 2.0;
                                     
                                     vec4 pinned = vec4(b.x > 1.0, b.y > 1.0, b.z > 1.0, b.w > 1.0);
                                     b = min(b, 1.0);
                                     vec4 f_Out;
                                     vec4 f_In;
                                     
                                     f_Out = texture(Dist2Map, Tex0);
                                     f_In.x = texture(Dist2Map, TexS_SW.zw).x;
                                     f_In.y = texture(Dist2Map, TexW_NW.zw).y;
                                     f_In.z = texture(Dist2Map, TexE_SE.zw).z;
                                     f_In.w = texture(Dist2Map, TexN_NE.zw).w;
                                     
                                     vec4 OUT = mix(f_In, f_Out.wzyx, b);
                                     
                                     fragColor = max(OUT - pinned * Evapor_b, 0.0);
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
