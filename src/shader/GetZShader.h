//
//  GetZShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_GetZShader_h
#define ofxInkSimExample_GetZShader_h

#include "ofMain.h"
#include "InkShader.h"

class GetZShader : public InkShader
{
public:
    
    GetZShader()
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
                ofTexture& texRef)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniformTexture("src", texRef, 0);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D src;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 a = texture2D(src, Tex0);
                                     
                                     gl_FragColor = vec4(1.0 - a.z, 1.0 - a.z, 1.0 - a.z, 1.0);
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
                                 uniform sampler2D src;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 a = texture(src, Tex0);
                                     
                                     fragColor = vec4(1.0 - a.z, 1.0 - a.z, 1.0 - a.z, 1.0);
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
