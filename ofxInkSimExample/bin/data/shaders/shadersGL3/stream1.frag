#pragma include "./header.glslinc"

uniform sampler2D MiscMap;
uniform sampler2D Dist1Map;

uniform float Evapor_b;
uniform vec2 offset;

in VSOUT
{
    vec2 texCoord;
} fsin;

out vec4 fragColor;

void main(void)
{
    float dx = offset.s;
    float dy = offset.t;
    
    vec2 Tex0 = fsin.texCoord;

    vec4 TexN_NE = vec4(Tex0.x,      Tex0.y - dy, Tex0.x + dx, Tex0.y - dy);
    vec4 TexE_SE = vec4(Tex0.x + dx, Tex0.y,      Tex0.x + dx, Tex0.y + dy);
    vec4 TexW_NW = vec4(Tex0.x - dx, Tex0.y,      Tex0.x - dx, Tex0.y - dy);
    vec4 TexS_SW = vec4(Tex0.x,      Tex0.y + dy, Tex0.x - dx, Tex0.y + dy);
    
    float b0 = texture(MiscMap, Tex0).x;
    float bN = texture(MiscMap, TexN_NE.xy).x;
    float bE = texture(MiscMap, TexE_SE.xy).x;
    float bW = texture(MiscMap, TexW_NW.xy).x;
    float bS = texture(MiscMap, TexS_SW.xy).x;
    
    vec4 b = vec4(bS, bW, bE, bN);
    b = (b + b0) / 2.0;
    
    vec4 pinned = vec4(b.x > 1.0, b.y > 1.0, b.z > 1.0, b.w > 1.0);
    b = min(b, 1.0);
    vec4 f_Out, f_In;
    
    f_Out = texture(Dist1Map, Tex0);
    f_In.x = texture(Dist1Map, TexS_SW.xy).x;
    f_In.y = texture(Dist1Map, TexW_NW.xy).y;
    f_In.z = texture(Dist1Map, TexE_SE.xy).z;
    f_In.w = texture(Dist1Map, TexN_NE.xy).w;
    
    vec4 OUT = mix(f_In, f_Out.wzyx, b);
    
    fragColor = max(OUT - pinned * Evapor_b, 0.0);
}