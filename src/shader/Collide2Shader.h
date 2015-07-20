//
//  Collide2Shader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_Collide2Shader_h
#define ofxInkSimExample_Collide2Shader_h

#include "ofMain.h"
#include "InkShader.h"

class Collide2Shader : public InkShader
{
public:
    
    Collide2Shader()
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
                float omega,
                ofTexture& VelDen,
                ofTexture& Dist2,
                ofTexture& FlowInk)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniform1f("A", 0.02777778f);
        shader.setUniform1f("B", 0.08333334f);
        shader.setUniform1f("C", 0.125f);
        shader.setUniform1f("D", 0.04166667f);
        shader.setUniform1f("advect_p", advect_p);
        shader.setUniform1f("Omega", omega);
        shader.setUniformTexture("VelDenMap", VelDen, 0);
        shader.setUniformTexture("Dist2Map", Dist2, 1);
        shader.setUniformTexture("InkMap", FlowInk, 2);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform float A;
                                 uniform float B;
                                 uniform float C;
                                 uniform float D;
                                 uniform float advect_p;
                                 uniform float Omega;
                                 
                                 uniform sampler2D VelDenMap;
                                 uniform sampler2D Dist2Map;
                                 uniform sampler2D InkMap;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 VelDen = texture2D(VelDenMap, Tex0);
                                     vec2 v = VelDen.xy;
                                     float p = VelDen.z;
                                     
                                     vec4 eiDotv = vec4(-v.y + v.x, v.y + v.x, -v.y - v.x, v.y - v.x);
                                     float ad = smoothstep(0.0, advect_p,p);
                                     vec4 f_eq = A * p + ad * (B * eiDotv + C * eiDotv * eiDotv - D * dot(v, v));
                                     vec4 f = texture2D(Dist2Map, Tex0);
                                     
                                     gl_FragColor = mix(f, f_eq, Omega);
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
                                 uniform float A;
                                 uniform float B;
                                 uniform float C;
                                 uniform float D;
                                 uniform float advect_p;
                                 uniform float Omega;
                                 
                                 uniform sampler2D VelDenMap;
                                 uniform sampler2D Dist2Map;
                                 uniform sampler2D InkMap;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 VelDen = texture(VelDenMap, Tex0);
                                     vec2 v = VelDen.xy;
                                     float p = VelDen.z;
                                     
                                     vec4 eiDotv = vec4(-v.y + v.x, v.y + v.x, -v.y - v.x, v.y - v.x);
                                     float ad = smoothstep(0.0, advect_p,p);
                                     vec4 f_eq = A * p + ad * (B * eiDotv + C * eiDotv * eiDotv - D * dot(v, v));
                                     vec4 f = texture(Dist2Map, Tex0);
                                     
                                     fragColor = mix(f, f_eq, Omega);
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
