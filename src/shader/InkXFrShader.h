//
//  InkXFrShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_InkXFrShader_h
#define ofxInkSimExample_InkXFrShader_h

#include "ofMain.h"
#include "InkShader.h"

class InkXFrShader : public InkShader
{
public:
    
    InkXFrShader()
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
                ofTexture& FlowInk,
                ofTexture& SinkInk)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniformTexture("FlowInkMap", FlowInk, 0);
        shader.setUniformTexture("SinkInkMap", SinkInk, 1);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D FlowInkMap;
                                 uniform sampler2D SinkInkMap;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 sink = texture2D(SinkInkMap, Tex0);
                                     vec4 if0 = texture2D(FlowInkMap, Tex0);
                                     vec4 if_new = if0 - sink;
                                     
                                     gl_FragColor = if_new;
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
                                 uniform sampler2D FlowInkMap;
                                 uniform sampler2D SinkInkMap;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 sink = texture(SinkInkMap, Tex0);
                                     vec4 if0 = texture(FlowInkMap, Tex0);
                                     vec4 if_new = if0 - sink;
                                     
                                     fragColor = if_new;
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
