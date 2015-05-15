#pragma include "./header.glslinc"

uniform sampler2D test;
uniform sampler2D WaterSurface;

void main(void)
{
    vec2 Tex0 = gl_TexCoord[0].st;

    gl_FragColor = texture2D(WaterSurface, Tex0);
}