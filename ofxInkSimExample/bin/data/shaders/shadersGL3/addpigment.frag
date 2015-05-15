#pragma include "./header.glslinc"

uniform sampler2D SurfInk;
uniform sampler2D WaterSurface;
uniform sampler2D Misc;

uniform float gamma;
uniform float baseMask;

in VSOUT
{
    vec2 texCoord;
} fsin;

out vec4 fragColor;

void main(void)
{
    vec2 Tex0 = fsin.texCoord;
    
    vec4 is = texture(SurfInk, Tex0);
    vec4 wa = texture(WaterSurface, Tex0);
    vec4 mi = texture(Misc, Tex0);
    
    float DepMask = max(1.0 - mi.z / gamma, baseMask);
    
    float re = is.x + clamp(wa.x, 0.0, DepMask); 
    float gr = is.y + clamp(wa.y, 0.0, DepMask); 
    float bl = is.z + clamp(wa.z, 0.0, DepMask);
    
    fragColor = vec4(re, gr, bl, is.w);
}
