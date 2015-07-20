//
//  BlockShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_BlockShader_h
#define ofxInkSimExample_BlockShader_h

#include "ofMain.h"
#include "InkShader.h"

class BlockShader : public InkShader
{
public:
    
    BlockShader()
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
                float advect_p,
                float b11, float b12, float b13,
                float b21, float b22,
                float p1, float p2, float p3,
                float toe_p,
                float omega,
                ofVec2f offset,
                ofTexture& misc,
                ofTexture& velDen,
                ofTexture& flowInk,
                ofTexture& fixInk,
                ofTexture& disorder)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniform1f("A0", 0.4444444f);
        shader.setUniform1f("advect_p", advect_p);
        shader.setUniform3f("Blk_1", b11, b12, b13);
        shader.setUniform2f("Blk_2", b21, b22);
        shader.setUniform3f("Pin_w", p1, p2, p3);
        shader.setUniform1f("toe_p", toe_p);
        shader.setUniform1f("Omega", omega);
        shader.setUniform1f("Corn_mul", pow(2.0f, 0.5f));
        shader.setUniform2fv("offset", offset.getPtr());
        shader.setUniformTexture("MiscMap", misc, 0);
        shader.setUniformTexture("VelDenMap", velDen, 1);
        shader.setUniformTexture("FlowInkMap", flowInk, 2);
        shader.setUniformTexture("FixInkMap", fixInk, 3);
        shader.setUniformTexture("DisorderMap", disorder, 4);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform float A0;
                                 uniform float advect_p;
                                 uniform vec3 Blk_1;
                                 uniform vec2 Blk_2;
                                 uniform vec3 Pin_w;
                                 uniform float toe_p;
                                 uniform float Omega;
                                 uniform float Corn_mul;
                                 uniform vec2 offset;
                                 
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D VelDenMap;
                                 uniform sampler2D FlowInkMap;
                                 uniform sampler2D FixInkMap;
                                 uniform sampler2D DisorderMap;
                                 
                                 void main(void){
                                     
                                     float dx = offset.s;
                                     float dy = offset.t;
                                     
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 TexN_NE = vec4(Tex0.x,      Tex0.y - dy, Tex0.x + dx, Tex0.y - dy);
                                     vec4 TexE_SE = vec4(Tex0.x + dx, Tex0.y,      Tex0.x + dx, Tex0.y + dy);
                                     vec4 TexW_NW = vec4(Tex0.x - dx, Tex0.y,      Tex0.x - dx, Tex0.y - dy);
                                     vec4 TexS_SW = vec4(Tex0.x,      Tex0.y + dy, Tex0.x - dx, Tex0.y + dy);
                                     
                                     vec4 Misc0 = texture2D(MiscMap, Tex0);
                                     vec4 VelDen = texture2D(VelDenMap, Tex0);
                                     
                                     float f0 = Misc0.y;
                                     float ws = Misc0.w;
                                     vec2 v = VelDen.xy;
                                     float wf = VelDen.z;
                                     float seep = VelDen.w;
                                     
                                     float glue = texture2D(FlowInkMap, Tex0).w;
                                     float FixBlk = texture2D(FixInkMap, Tex0).w;
                                     
                                     vec4 Disorder = texture2D(DisorderMap, Tex0);
                                     
                                     // Derive ad: Less advection for lower wf
                                     float ad = smoothstep(0.0, advect_p, wf);
                                     
                                     // Derive f0
                                     float f0_eq = A0 * wf - ad * (2.0 / 3.0) * dot(v, v);
                                     f0 = mix(f0, f0_eq, Omega);
                                     
                                     float GrainBlock = Disorder.x;
                                     float AlumBlock = Disorder.z;
                                     float block = Blk_1.x + dot(Blk_1.yz, vec2(GrainBlock, AlumBlock)) + dot(Blk_2.xy, vec2(glue, FixBlk));
                                     block = min(1.0, block);
                                     bool pinning = (wf == 0.0);
                                     
                                     float Pindisor = mix(Disorder.x, Disorder.w, smoothstep(0.0, toe_p, glue));
                                     float pin = Pin_w.x + dot(Pin_w.yz, vec2(FixBlk, Pindisor));
                                     
                                     pinning = (pinning) && (texture2D(VelDenMap, TexN_NE.xy).z < pin);
                                     pinning = (pinning) && (texture2D(VelDenMap, TexE_SE.xy).z < pin);
                                     pinning = (pinning) && (texture2D(VelDenMap, TexW_NW.xy).z < pin);
                                     pinning = (pinning) && (texture2D(VelDenMap, TexS_SW.xy).z < pin);
                                     
                                     pin *= Corn_mul;
                                     
                                     pinning = (pinning) && (texture2D(VelDenMap, TexN_NE.zw).z < pin);
                                     pinning = (pinning) && (texture2D(VelDenMap, TexE_SE.zw).z < pin);
                                     pinning = (pinning) && (texture2D(VelDenMap, TexW_NW.zw).z < pin);
                                     pinning = (pinning) && (texture2D(VelDenMap, TexS_SW.zw).z < pin);
                                     
                                     if (pinning)
                                         block = 1.0/0.0;
                                     
                                     gl_FragColor = vec4(block, f0, wf, max(ws - seep, 0.0));
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
                                 uniform float A0;
                                 uniform float advect_p;
                                 uniform vec3 Blk_1;
                                 uniform vec2 Blk_2;
                                 uniform vec3 Pin_w;
                                 uniform float toe_p;
                                 uniform float Omega;
                                 uniform float Corn_mul;
                                 uniform vec2 offset;
                                 
                                 uniform sampler2D MiscMap;
                                 uniform sampler2D VelDenMap;
                                 uniform sampler2D FlowInkMap;
                                 uniform sampler2D FixInkMap;
                                 uniform sampler2D DisorderMap;
                                 
                                 in VSOUT
        {
            vec2 texCoord;
        } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void){
                                     
                                     float dx = offset.s;
                                     float dy = offset.t;
                                     
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 TexN_NE = vec4(Tex0.x,      Tex0.y - dy, Tex0.x + dx, Tex0.y - dy);
                                     vec4 TexE_SE = vec4(Tex0.x + dx, Tex0.y,      Tex0.x + dx, Tex0.y + dy);
                                     vec4 TexW_NW = vec4(Tex0.x - dx, Tex0.y,      Tex0.x - dx, Tex0.y - dy);
                                     vec4 TexS_SW = vec4(Tex0.x,      Tex0.y + dy, Tex0.x - dx, Tex0.y + dy);
                                     
                                     vec4 Misc0 = texture(MiscMap, Tex0);
                                     vec4 VelDen = texture(VelDenMap, Tex0);
                                     
                                     float f0 = Misc0.y;
                                     float ws = Misc0.w;
                                     vec2 v = VelDen.xy;
                                     float wf = VelDen.z;
                                     float seep = VelDen.w;
                                     
                                     float glue = texture(FlowInkMap, Tex0).w;
                                     float FixBlk = texture(FixInkMap, Tex0).w;
                                     
                                     vec4 Disorder = texture(DisorderMap, Tex0);
                                     
                                     // Derive ad: Less advection for lower wf
                                     float ad = smoothstep(0.0, advect_p, wf);
                                     
                                     // Derive f0
                                     float f0_eq = A0 * wf - ad * (2.0 / 3.0) * dot(v, v);
                                     f0 = mix(f0, f0_eq, Omega);
                                     
                                     float GrainBlock = Disorder.x;
                                     float AlumBlock = Disorder.z;
                                     float block = Blk_1.x + dot(Blk_1.yz, vec2(GrainBlock, AlumBlock)) + dot(Blk_2.xy, vec2(glue, FixBlk));
                                     block = min(1.0, block);
                                     bool pinning = (wf == 0.0);
                                     
                                     float Pindisor = mix(Disorder.x, Disorder.w, smoothstep(0.0, toe_p, glue));
                                     float pin = Pin_w.x + dot(Pin_w.yz, vec2(FixBlk, Pindisor));
                                     
                                     pinning = (pinning) && (texture(VelDenMap, TexN_NE.xy).z < pin);
                                     pinning = (pinning) && (texture(VelDenMap, TexE_SE.xy).z < pin);
                                     pinning = (pinning) && (texture(VelDenMap, TexW_NW.xy).z < pin);
                                     pinning = (pinning) && (texture(VelDenMap, TexS_SW.xy).z < pin);
                                     
                                     pin *= Corn_mul;
                                     
                                     pinning = (pinning) && (texture(VelDenMap, TexN_NE.zw).z < pin);
                                     pinning = (pinning) && (texture(VelDenMap, TexE_SE.zw).z < pin);
                                     pinning = (pinning) && (texture(VelDenMap, TexW_NW.zw).z < pin);
                                     pinning = (pinning) && (texture(VelDenMap, TexS_SW.zw).z < pin);
                                     
                                     if (pinning)
                                         block = 1.0/0.0;
                                     
                                     fragColor = vec4(block, f0, wf, max(ws - seep, 0.0));
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
