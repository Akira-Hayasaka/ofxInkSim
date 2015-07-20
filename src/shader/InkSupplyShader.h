//
//  InkSupplyShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_InkSupplyShader_h
#define ofxInkSimExample_InkSupplyShader_h

#include "ofMain.h"
#include "InkShader.h"

class InkSupplyShader : public InkShader
{
public:
    
    InkSupplyShader()
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
                ofTexture& velDen,
                ofTexture& surfInk,
                ofTexture& misc)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniformTexture("VelDenMap", velDen, 0);
        shader.setUniformTexture("SurfInkMap", surfInk, 1);
        shader.setUniformTexture("MiscMap", misc, 2);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D VelDenMap;
                                 uniform sampler2D SurfInkMap;
                                 uniform sampler2D MiscMap;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 is = texture2D(SurfInkMap, Tex0);
                                     float ws = texture2D(MiscMap, Tex0).w;
                                     float seep = texture2D(VelDenMap, Tex0).w;
                                     
                                     is = (ws <= 0.001) ? vec4(0.0, 0.0, 0.0, 0.0) : is;
                                     
                                     gl_FragColor = is;
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
                                 uniform sampler2D SurfInkMap;
                                 uniform sampler2D MiscMap;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 is = texture(SurfInkMap, Tex0);
                                     float ws = texture(MiscMap, Tex0).w;
                                     float seep = texture(VelDenMap, Tex0).w;
                                     
                                     is = (ws <= 0.001) ? vec4(0.0, 0.0, 0.0, 0.0) : is;
                                     
                                     fragColor = is;
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
