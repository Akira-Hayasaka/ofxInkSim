#pragma include "./header.glslinc"

uniform sampler2D FixInkMap;
uniform sampler2D SinkInkMap;

void main(void)
{
    vec2 Tex0 = gl_TexCoord[0].st;
    
    vec4 sink = texture2D(SinkInkMap, Tex0);
    vec4 ix0 = texture2D(FixInkMap, Tex0);
    vec4 ix_new = sink + ix0;
    ix_new = vec4(ix_new.xyz, (ix_new.x + ix_new.y + ix_new.z) / 3.0);
    
    gl_FragColor = ix_new;
}