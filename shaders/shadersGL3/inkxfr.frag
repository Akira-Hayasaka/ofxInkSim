#pragma include "./header.glslinc"

uniform sampler2D FlowInkMap;
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
    vec4 if0 = texture(FlowInkMap, Tex0);
    vec4 if_new = if0 - sink;
    
    fragColor = if_new;
}