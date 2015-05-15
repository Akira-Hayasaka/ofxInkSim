#pragma include "./header.glslinc"

uniform sampler2D VelDenMap;
uniform sampler2D MiscMap;
uniform sampler2D FlowInkMap;
uniform sampler2D FixInkMap;

uniform vec3 FixRate;

void main(void)
{
    vec2 Tex0 = gl_TexCoord[0].st;
    
    float lwf = texture2D(MiscMap, Tex0).z;
    float wf = texture2D(VelDenMap, Tex0).z;
    vec4 if0 = texture2D(FlowInkMap, Tex0);
    
    float wLoss = max(lwf - wf, 0.0);
    float FixFactor;
    
    if (wLoss > 0.0)
        FixFactor = wLoss / lwf;
    else
        FixFactor = 0.0;
    
    float mu_star = clamp(FixRate.y + FixRate.z * if0.w, 0.0, 1.0);
    FixFactor = max(FixFactor * (1.0 - smoothstep(0.0, mu_star, wf)), FixRate.x);
    
    vec4 sink = if0 * FixFactor;
    
    gl_FragColor = sink;
}