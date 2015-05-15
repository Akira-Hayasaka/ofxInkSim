#pragma include "./header.glslinc"

uniform vec2 pxSize;

void main()
{
    gl_TexCoord[0] = vec4(gl_MultiTexCoord0.xy * pxSize, 0.0, 0.0);
    gl_Position = ftransform();
}