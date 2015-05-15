#pragma include "./header.glslinc"

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
