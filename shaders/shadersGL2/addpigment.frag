#pragma include "./header.glslinc"

uniform sampler2D SurfInk;
uniform sampler2D WaterSurface;
uniform sampler2D Misc;
uniform sampler2D test;

uniform float gamma;
uniform float baseMask;

void main(void)
{
    vec2 Tex0 = gl_TexCoord[0].st;
    
    vec4 is = texture2D(SurfInk, Tex0);
    vec4 wa = texture2D(WaterSurface, Tex0);
    vec4 mi = texture2D(Misc, Tex0);
    
    float DepMask = max(1.0 - mi.z / gamma, baseMask);
    
    float re = is.x + clamp(wa.x, 0.0, DepMask);
    float gr = is.y + clamp(wa.y, 0.0, DepMask);
    float bl = is.z + clamp(wa.z, 0.0, DepMask);
    
    gl_FragColor = vec4(re, gr, bl, is.w);
}
