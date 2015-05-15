#pragma include "./header.glslinc"

uniform float A;
uniform float B;
uniform float C;
uniform float D;
uniform float advect_p;
uniform float Omega;

uniform sampler2D VelDenMap;
uniform sampler2D Dist2Map;
uniform sampler2D InkMap;

in VSOUT
{
    vec2 texCoord;
} fsin;

out vec4 fragColor;

void main(void)
{
    vec2 Tex0 = fsin.texCoord;
    
    vec4 VelDen = texture(VelDenMap, Tex0);
    vec2 v = VelDen.xy;
    float p = VelDen.z;
    
    vec4 eiDotv = vec4(-v.y + v.x, v.y + v.x, -v.y - v.x, v.y - v.x);
    float ad = smoothstep(0.0, advect_p,p);
    vec4 f_eq = A * p + ad * (B * eiDotv + C * eiDotv * eiDotv - D * dot(v, v));
    vec4 f = texture(Dist2Map, Tex0);
    
    fragColor = mix(f, f_eq, Omega);
}