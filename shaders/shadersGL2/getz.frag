#pragma include "./header.glslinc"

uniform sampler2D src;

void main(void)
{
    vec2 Tex0 = gl_TexCoord[0].st;
    
    vec4 a = texture2D(src, Tex0);
    
    gl_FragColor = vec4(1.0 - a.z, 1.0 - a.z, 1.0 - a.z, 1.0);
}
