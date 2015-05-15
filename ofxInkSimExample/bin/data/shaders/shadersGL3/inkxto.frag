#pragma include "./header.glslinc"

uniform sampler2D FixInkMap;
uniform sampler2D SinkInkMap;

in VSOUT
{
    vec2 texCoord;
} fsin;

out vec4 fragColor;

void main(void)
{
    vec2 Tex0 = fsin.texCoord;
    
    vec4 sink = texture(SinkInkMap, Tex0);
    vec4 ix0 = texture(FixInkMap, Tex0);
    vec4 ix_new = sink + ix0;
    ix_new = vec4(ix_new.xyz, (ix_new.x + ix_new.y + ix_new.z) / 3.0);
    
    fragColor = ix_new;
}