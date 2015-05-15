#pragma include "./header.glslinc"

uniform sampler2D FlowInkMap;
uniform sampler2D SinkInkMap;

void main(void)
{
    vec2 Tex0 = gl_TexCoord[0].st;
    
    vec4 sink = texture2D(SinkInkMap, Tex0);
    vec4 if0 = texture2D(FlowInkMap, Tex0);
    vec4 if_new = if0 - sink;
    
    gl_FragColor = if_new;
}