#pragma include "./header.glslinc"

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
