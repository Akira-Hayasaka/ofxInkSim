//
//  GapShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_GapShader_h
#define ofxInkSimExample_GapShader_h

#include "ofMain.h"
#include "InkShader.h"

class GapShader : public InkShader
{
public:
    
    GapShader()
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
                ofTexture Grain,
                ofTexture Alum,
                ofTexture Pinning)
    {
        shader.begin();
        shader.setUniform2fv("pxSize", pxSize.getPtr());
        shader.setUniformTexture("Grain", Grain, 0);
        shader.setUniformTexture("Alum", Alum, 1);
        shader.setUniformTexture("Pinning", Pinning, 2);
        drawPlane(w, h);
        shader.end();
    }
    
protected:
    
    void glTwo()
    {
        fragmentShader = GLSL120(
                                 uniform sampler2D Grain;
                                 uniform sampler2D Alum;
                                 uniform sampler2D Pinning;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = gl_TexCoord[0].st;
                                     
                                     vec4 g = texture2D(Grain, Tex0);
                                     vec4 a = texture2D(Alum, Tex0);
                                     vec4 p = texture2D(Pinning, Tex0);
                                     
                                     float gg = (g.x + g.y + g.z) / 3.0;
                                     float aa = (a.x + a.y + a.z) / 3.0;
                                     float pp = (p.x + p.y + p.z) / 3.0;
                                     float co = 1.0;
                                     
                                     gg *= co;
                                     aa *= co;
                                     pp *= co;
                                     
                                     gl_FragColor = vec4(gg, 1.0, aa, pp);
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
                                 uniform sampler2D Grain;
                                 uniform sampler2D Alum;
                                 uniform sampler2D Pinning;
                                 
                                 in VSOUT
                                 {
                                     vec2 texCoord;
                                 } fsin;
                                 
                                 out vec4 fragColor;
                                 
                                 void main(void)
                                 {
                                     vec2 Tex0 = fsin.texCoord;
                                     
                                     vec4 g = texture(Grain, Tex0);
                                     vec4 a = texture(Alum, Tex0);
                                     vec4 p = texture(Pinning, Tex0);
                                     
                                     float gg = (g.x + g.y + g.z) / 3.0;
                                     float aa = (a.x + a.y + a.z) / 3.0;
                                     float pp = (p.x + p.y + p.z) / 3.0;
                                     float co = 1.0;
                                     
                                     gg *= co;
                                     aa *= co;
                                     pp *= co;
                                     
                                     fragColor = vec4(gg, 1.0, aa, pp);
                                 }
                                 );
        
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
        shader.bindDefaults();
        shader.linkProgram();
    }
};

#endif
