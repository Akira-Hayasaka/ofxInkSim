//
//  Stream1Shader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_Stream1Shader_h
#define ofxInkSimExample_Stream1Shader_h

#include "ofMain.h"
#include "InkShader.h"

class Stream1Shader : public InkShader
{
public:
    
    Stream1Shader()
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
                ofTexture& Misc,
                ofTexture& Dist1)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniform1f("Evapor_b", evapor_b);
        shader.setUniform2fv("offset", offset.getPtr());
        shader.setUniformTexture("MiscMap", Misc, 0);
        shader.setUniformTexture("Dist1Map", Dist1, 1);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D Dist1Map;
                                 
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
                                     float bN = texture2D(MiscMap, TexN_NE.xy).x;
                                     float bE = texture2D(MiscMap, TexE_SE.xy).x;
                                     float bW = texture2D(MiscMap, TexW_NW.xy).x;
                                     float bS = texture2D(MiscMap, TexS_SW.xy).x;
                                     
                                     vec4 b = vec4(bS, bW, bE, bN);
                                     b = (b + b0) / 2.0;
                                     
                                     vec4 pinned = vec4(b.x > 1.0, b.y > 1.0, b.z > 1.0, b.w > 1.0);
                                     b = min(b, 1.0);
                                     vec4 f_Out;
                                     vec4 f_In;
                                     
                                     f_Out = texture2D(Dist1Map, Tex0);
                                     f_In.x = texture2D(Dist1Map, TexS_SW.xy).x;
                                     f_In.y = texture2D(Dist1Map, TexW_NW.xy).y;
                                     f_In.z = texture2D(Dist1Map, TexE_SE.xy).z;
                                     f_In.w = texture2D(Dist1Map, TexN_NE.xy).w;
                                     
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
                                 uniform sampler2D Dist1Map;
                                 
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
                                     float bN = texture(MiscMap, TexN_NE.xy).x;
                                     float bE = texture(MiscMap, TexE_SE.xy).x;
                                     float bW = texture(MiscMap, TexW_NW.xy).x;
                                     float bS = texture(MiscMap, TexS_SW.xy).x;
                                     
                                     vec4 b = vec4(bS, bW, bE, bN);
                                     b = (b + b0) / 2.0;
                                     
                                     vec4 pinned = vec4(b.x > 1.0, b.y > 1.0, b.z > 1.0, b.w > 1.0);
                                     b = min(b, 1.0);
                                     vec4 f_Out;
                                     vec4 f_In;
                                     
                                     f_Out = texture(Dist1Map, Tex0);
                                     f_In.x = texture(Dist1Map, TexS_SW.xy).x;
                                     f_In.y = texture(Dist1Map, TexW_NW.xy).y;
                                     f_In.z = texture(Dist1Map, TexE_SE.xy).z;
                                     f_In.w = texture(Dist1Map, TexN_NE.xy).w;
                                     
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
