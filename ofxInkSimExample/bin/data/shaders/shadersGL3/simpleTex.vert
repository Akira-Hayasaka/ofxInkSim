#pragma include "./header.glslinc"

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