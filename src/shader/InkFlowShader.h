//
//  InkFlowShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_InkFlowShader_h
#define ofxInkSimExample_InkFlowShader_h

#include "ofMain.h"
#include "InkShader.h"

class InkFlowShader : public InkShader
{
public:
    
    InkFlowShader()
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
                float ba1, float ba2,
                ofVec2f offset,
                ofTexture& velDen,
                ofTexture& misc,
                ofTexture& dist1,
                ofTexture& dist2,
                ofTexture& flowInk,
                ofTexture& surfInk)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniform2f("Blk_a", ba1, ba2);
        shader.setUniform2fv("offset", offset.getPtr());
        shader.setUniformTexture("VelDenMap", velDen, 0);
        shader.setUniformTexture("MiscMap", misc, 1);
        shader.setUniformTexture("Dist1Map", dist1, 2);
        shader.setUniformTexture("Dist2Map", dist2, 3);
        shader.setUniformTexture("FlowInkMap", flowInk, 4);
        shader.setUniformTexture("SurfInkMap", surfInk, 5);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D VelDenMap;
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D Dist1Map;
                                 uniform sampler2D Dist2Map;
                                 uniform sampler2D FlowInkMap;
                                 uniform sampler2D SurfInkMap;
                                 
                                 uniform vec2 Blk_a;
                                 uniform vec2 offset;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     float dx=offset.x;
                                     float dy=offset.y;
                                     
                                     vec4 TexN_NE = vec4(Tex0.x,      Tex0.y - dy, Tex0.x + dx, Tex0.y - dy);
                                     vec4 TexE_SE = vec4(Tex0.x + dx, Tex0.y,      Tex0.x + dx, Tex0.y + dy);
                                     vec4 TexW_NW = vec4(Tex0.x - dx, Tex0.y,      Tex0.x - dx, Tex0.y - dy);
                                     vec4 TexS_SW = vec4(Tex0.x,      Tex0.y + dy, Tex0.x - dx, Tex0.y + dy);
                                     
                                     vec2 u = texture2D(VelDenMap, Tex0).xy;
                                     float seep = texture2D(VelDenMap, Tex0).w;
                                     
                                     vec2 ucoord = vec2(dx * u.x, dy * u.y);
                                     
                                     float f1 = texture2D(Dist1Map, Tex0).x;
                                     float f2 = texture2D(Dist1Map, Tex0).y;
                                     float f3 = texture2D(Dist1Map, Tex0).z;
                                     float f4 = texture2D(Dist1Map, Tex0).w;
                                     
                                     float f5 = texture2D(Dist2Map, Tex0).x;
                                     float f6 = texture2D(Dist2Map, Tex0).y;
                                     float f7 = texture2D(Dist2Map, Tex0).z;
                                     float f8 = texture2D(Dist2Map, Tex0).w;
                                     
                                     float wf = texture2D(VelDenMap, Tex0).z;
                                     float lwf = texture2D(MiscMap, Tex0).z;
                                     
                                     vec4 pf1 = texture2D(FlowInkMap, TexS_SW.xy);
                                     vec4 pf2 = texture2D(FlowInkMap, TexW_NW.xy);
                                     vec4 pf3 = texture2D(FlowInkMap, TexE_SE.xy);
                                     vec4 pf4 = texture2D(FlowInkMap, TexN_NE.xy);
                                     
                                     vec4 pf5 = texture2D(FlowInkMap, TexS_SW.zw);
                                     vec4 pf6 = texture2D(FlowInkMap, TexW_NW.zw);
                                     vec4 pf7 = texture2D(FlowInkMap, TexE_SE.zw);
                                     vec4 pf8 = texture2D(FlowInkMap, TexN_NE.zw);
                                     
                                     vec4 iFill;
                                     vec4 iStream;
                                     vec4 is = texture2D(SurfInkMap, Tex0);
                                     
                                     bool JustFilled;
                                     vec4 pf;
                                     vec4 ia;
                                     
                                     if (lwf == 0.0 && wf != 0.0)
                                         JustFilled = true;
                                     else
                                         JustFilled = false;
                                     
                                     iFill = (f1*pf1+f2*pf2+f3*pf3+f4*pf4+f5*pf5+f6*pf6+f7*pf7+f8*pf8) / wf;
                                     
                                     if (abs(u.x) < dx && abs(u.y) < dy)
                                         iStream = texture2D(FlowInkMap, Tex0);
                                     else
                                         iStream = texture2D(FlowInkMap, Tex0 - u);
                                     
                                     float Blk_hindrance = mix(1.0, Blk_a.x, smoothstep(0.0, Blk_a.y, sqrt(u.x * u.x + u.y * u.y)));
                                     ia = mix((JustFilled) ? iFill : iStream, texture2D(FlowInkMap, Tex0), Blk_hindrance);
                                     
                                     if (seep != 0.0)
                                         pf = (ia * lwf + is * seep) / (lwf + seep);
                                     else
                                         pf = ia;
                                     
                                     gl_FragColor = pf;
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
                                 uniform sampler2D Dist1Map;
                                 uniform sampler2D Dist2Map;
                                 uniform sampler2D FlowInkMap;
                                 uniform sampler2D SurfInkMap;
                                 
                                 uniform vec2 Blk_a;
                                 uniform vec2 offset;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     float dx=offset.x;
                                     float dy=offset.y;
                                     
                                     vec4 TexN_NE = vec4(Tex0.x,      Tex0.y - dy, Tex0.x + dx, Tex0.y - dy);
                                     vec4 TexE_SE = vec4(Tex0.x + dx, Tex0.y,      Tex0.x + dx, Tex0.y + dy);
                                     vec4 TexW_NW = vec4(Tex0.x - dx, Tex0.y,      Tex0.x - dx, Tex0.y - dy);
                                     vec4 TexS_SW = vec4(Tex0.x,      Tex0.y + dy, Tex0.x - dx, Tex0.y + dy);
                                     
                                     vec2 u = texture(VelDenMap, Tex0).xy;
                                     float seep = texture(VelDenMap, Tex0).w;
                                     
                                     vec2 ucoord = vec2(dx * u.x, dy * u.y);
                                     
                                     float f1 = texture(Dist1Map, Tex0).x;
                                     float f2 = texture(Dist1Map, Tex0).y;
                                     float f3 = texture(Dist1Map, Tex0).z;
                                     float f4 = texture(Dist1Map, Tex0).w;
                                     
                                     float f5 = texture(Dist2Map, Tex0).x;
                                     float f6 = texture(Dist2Map, Tex0).y;
                                     float f7 = texture(Dist2Map, Tex0).z;
                                     float f8 = texture(Dist2Map, Tex0).w;
                                     
                                     float wf = texture(VelDenMap, Tex0).z;
                                     float lwf = texture(MiscMap, Tex0).z;
                                     
                                     vec4 pf1 = texture(FlowInkMap, TexS_SW.xy);
                                     vec4 pf2 = texture(FlowInkMap, TexW_NW.xy);
                                     vec4 pf3 = texture(FlowInkMap, TexE_SE.xy);
                                     vec4 pf4 = texture(FlowInkMap, TexN_NE.xy);
                                     
                                     vec4 pf5 = texture(FlowInkMap, TexS_SW.zw);
                                     vec4 pf6 = texture(FlowInkMap, TexW_NW.zw);
                                     vec4 pf7 = texture(FlowInkMap, TexE_SE.zw);
                                     vec4 pf8 = texture(FlowInkMap, TexN_NE.zw);
                                     
                                     vec4 iFill;
                                     vec4 iStream;
                                     vec4 is = texture(SurfInkMap, Tex0);
                                     
                                     bool JustFilled;
                                     vec4 pf;
                                     vec4 ia;
                                     
                                     if (lwf == 0.0 && wf != 0.0)
                                         JustFilled = true;
                                     else
                                         JustFilled = false;
                                     
                                     iFill = (f1*pf1+f2*pf2+f3*pf3+f4*pf4+f5*pf5+f6*pf6+f7*pf7+f8*pf8) / wf;
                                     
                                     if (abs(u.x) < dx && abs(u.y) < dy)
                                         iStream = texture(FlowInkMap, Tex0);
                                     else
                                         iStream = texture(FlowInkMap, Tex0 - u);
                                     
                                     float Blk_hindrance = mix(1.0, Blk_a.x, smoothstep(0.0, Blk_a.y, sqrt(u.x * u.x + u.y * u.y)));
                                     ia = mix((JustFilled) ? iFill : iStream, texture(FlowInkMap, Tex0), Blk_hindrance);
                                     
                                     if (seep != 0.0)
                                         pf = (ia * lwf + is * seep) / (lwf + seep);
                                     else
                                         pf = ia;
                                     
                                     fragColor = pf;
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
