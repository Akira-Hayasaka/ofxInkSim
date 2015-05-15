#pragma include "./header.glslinc"

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
