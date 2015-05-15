#pragma include "./header.glslinc"

uniform sampler2D test;
uniform sampler2D WaterSurface;

in VSOUT
{
    vec2 texCoord;
} fsin;

out vec4 fragColor;

void main(void)
{
    vec2 Tex0 = fsin.texCoord;

    fragColor = texture(WaterSurface, Tex0);
}