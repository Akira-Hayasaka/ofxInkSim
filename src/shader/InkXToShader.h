//
//  InkXToShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_InkXToShader_h
#define ofxInkSimExample_InkXToShader_h

#include "ofMain.h"
#include "InkShader.h"

class InkXToShader : public InkShader
{
public:
    
    InkXToShader()
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
                ofTexture& fixInk,
                ofTexture& sinkInk,
                ofTexture& velDen,
                float bEvaporToDisapper)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniformTexture("FixInkMap", fixInk, 0);
        shader.setUniformTexture("SinkInkMap", sinkInk, 1);
        shader.setUniformTexture("velDen", velDen, 2);
        shader.setUniform1f("bEvaporToDisapper", bEvaporToDisapper);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D FixInkMap;
                                 uniform sampler2D SinkInkMap;
                                 uniform sampler2D velDen;
                                 uniform float bEvaporToDisapper; // false = 0, true = 1
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 sink = texture2D(SinkInkMap, Tex0);
                                     vec4 ix0 = texture2D(FixInkMap, Tex0);
                                     vec4 ix_new = sink + ix0;
                                     ix_new = vec4(ix_new.xyz, (ix_new.x + ix_new.y + ix_new.z) / 3.0);
                                     
                                     if (bEvaporToDisapper == 1.0)
                                     {
                                         vec4 a = texture2D(velDen, Tex0);
                                         if (a.z <= 0.001)
                                             gl_FragColor = vec4(ix_new.rgb * (a.z * 990.0), ix_new.a);
                                         else
                                             gl_FragColor = ix_new;
                                     }
                                     else
                                     {
                                         gl_FragColor = ix_new;
                                     }
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
                                 uniform sampler2D FixInkMap;
                                 uniform sampler2D SinkInkMap;
                                 uniform sampler2D velDen;
                                 uniform float bEvaporToDisapper; // false = 0, true = 1
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 sink = texture(SinkInkMap, Tex0);
                                     vec4 ix0 = texture(FixInkMap, Tex0);
                                     vec4 ix_new = sink + ix0;
                                     ix_new = vec4(ix_new.xyz, (ix_new.x + ix_new.y + ix_new.z) / 3.0);
                                     
                                     if (bEvaporToDisapper == 1.0)
                                     {
                                         vec4 a = texture(velDen, Tex0);
                                         if (a.z <= 0.001)
                                             fragColor = vec4(ix_new.rgb * (a.z * 990.0), ix_new.a);
                                         else
                                             fragColor = ix_new;
                                     }
                                     else
                                     {
                                         fragColor = ix_new;
                                     }
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
