#pragma include "./header.glslinc"

uniform sampler2D Misc;
uniform sampler2D WaterSurface;

uniform float gamma;
uniform float baseMask;
uniform float waterAmount;

in VSOUT
{
    vec2 texCoord;
} fsin;

out vec4 fragColor;

void main(void)
{
    vec2 Tex0 = fsin.texCoord;
    
    vec4 mi = texture(Misc, Tex0);
    vec4 wa = texture(WaterSurface, Tex0);
    float DepMask = max(1.0 - mi.z / gamma, baseMask);
    
    float temp_waterAmount = 0.0;
    
    if (wa.w != 0.0)
    {
        temp_waterAmount = waterAmount;
    }
    
    fragColor = vec4(mi.xyz, mi.w + clamp(temp_waterAmount, 0.0, DepMask));
}
