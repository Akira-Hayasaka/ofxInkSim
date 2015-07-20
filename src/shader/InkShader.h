//
//  InkShader.h
//  ofxInkSimExample
//
//  Created by Akira on 7/20/15.
//
//

#ifndef ofxInkSimExample_InkShader_h
#define ofxInkSimExample_InkShader_h

#include "ofMain.h"

#define GLSL120(shader) "" #shader
#define GLSL150(shader) "#version 150 \n precision highp float; \n" #shader

class InkShader
{
public:
    
    InkShader()
    {
        if (ofGetGLProgrammableRenderer())
        {
            isProgrammableRenderer = true;
        }
        else
        {
            isProgrammableRenderer = false;
        }
        
        if (isProgrammableRenderer)
        {
            vertexShader = GLSL150(
                                   uniform mat4 projectionMatrix;
                                   uniform mat4 modelViewMatrix;
                                   uniform mat4 textureMatrix;
                                   uniform mat4 modelViewProjectionMatrix;
                                   uniform vec2 pxSize;
                                   
                                   in vec4  position;
                                   in vec2  texcoord;
                                   in vec4  color;
                                   in vec3  normal;
                                   
                                   out VSOUT
                                   {
                                       vec2 texCoord;
                                   } vsout;
                                   
                                   void main()
                                   {
                                       vsout.texCoord = texcoord * pxSize;
                                       gl_Position = modelViewProjectionMatrix * position;
                                   }
                                   );
            
            fragmentShader = GLSL150(
                                     out vec4 fragColor;
                                     
                                     void main()
                                     {
                                         fragColor = vec4(0.0, 1.0, 0.0, 1.0);
                                     }
                                     );
        }
        else
        {
            vertexShader = GLSL120(
                                   uniform vec2 pxSize;
                                   
                                   void main()
                                   {
                                       gl_TexCoord[0] = vec4(gl_MultiTexCoord0.xy * pxSize, 0.0, 0.0);
                                       gl_Position = ftransform();
                                   }
                                   );
            
            fragmentShader = GLSL120(
                                     void main()
                                     {
                                         gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
                                     }
                                     );
        }
        
        quad.getVertices().resize(4);
        quad.getTexCoords().resize(4);
        quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    }
    
    ~InkShader() { shader.unload(); }
    
protected:
    
    void drawPlane(float _width, float _height)
    {
        quad.setVertex(0, ofVec3f(0, 0, 0));
        quad.setVertex(1, ofVec3f(_width, 0, 0));
        quad.setVertex(2, ofVec3f(_width, _height, 0));
        quad.setVertex(3, ofVec3f(0, _height, 0));
        
        quad.setTexCoord(0, ofVec2f(0, 0));
        quad.setTexCoord(1, ofVec2f(_width, 0));
        quad.setTexCoord(2, ofVec2f(_width, _height));
        quad.setTexCoord(3, ofVec2f(0, _height));
        
        quad.draw();
    }
    
    ofVboMesh quad;
    ofShader shader;
    string fragmentShader;
    string vertexShader;
    bool isProgrammableRenderer;
};

#endif
