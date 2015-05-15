#pragma include "./header.glslinc"

uniform sampler2D VelDenMap;
uniform sampler2D SurfInkMap;
uniform sampler2D MiscMap;

in VSOUT
{
    vec2 texCoord;
} fsin;

out vec4 fragColor;

void main(void)
{
    vec2 Tex0 = fsin.texCoord;
    
    vec4 is = texture(SurfInkMap, Tex0);
    float ws = texture(MiscMap, Tex0).w;
    float seep = texture(VelDenMap, Tex0).w;
    
    is = (ws <= 0.001) ? vec4(0.0, 0.0, 0.0, 0.0) : is;
    
    fragColor = is;
}