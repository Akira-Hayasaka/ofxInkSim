#pragma include "./header.glslinc"

uniform sampler2D VelDenMap;
uniform sampler2D SurfInkMap;
uniform sampler2D MiscMap;

void main(void)
{
    vec2 Tex0 = gl_TexCoord[0].st;
    
    vec4 is = texture2D(SurfInkMap, Tex0);
    float ws = texture2D(MiscMap, Tex0).w;
    float seep = texture2D(VelDenMap, Tex0).w;
    
    is = (ws <= 0.001) ? vec4(0.0, 0.0, 0.0, 0.0) : is;
    
    gl_FragColor = is;
}