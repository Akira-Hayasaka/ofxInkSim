#pragma include "./header.glslinc"

uniform float A0;
uniform float advect_p;
uniform vec3 Blk_1;
uniform vec2 Blk_2;
uniform vec3 Pin_w;
uniform float toe_p;
uniform float Omega;
uniform float Corn_mul;
uniform vec2 offset;

uniform sampler2D MiscMap;
uniform sampler2D VelDenMap;
uniform sampler2D FlowInkMap;
uniform sampler2D FixInkMap;
uniform sampler2D DisorderMap;

in VSOUT
{
    vec2 texCoord;
} fsin;

out vec4 fragColor;

void main(void){
    
    float dx = offset.s;
    float dy = offset.t;
    
    vec2 Tex0 = fsin.texCoord;
    
    vec4 TexN_NE = vec4(Tex0.x,      Tex0.y - dy, Tex0.x + dx, Tex0.y - dy);
    vec4 TexE_SE = vec4(Tex0.x + dx, Tex0.y,      Tex0.x + dx, Tex0.y + dy);
    vec4 TexW_NW = vec4(Tex0.x - dx, Tex0.y,      Tex0.x - dx, Tex0.y - dy);
    vec4 TexS_SW = vec4(Tex0.x,      Tex0.y + dy, Tex0.x - dx, Tex0.y + dy);
    
    vec4 Misc0 = texture(MiscMap, Tex0);
    vec4 VelDen = texture(VelDenMap, Tex0);
    
    float f0 = Misc0.y;
    float ws = Misc0.w;
    vec2 v = VelDen.xy;
    float wf = VelDen.z;
    float seep = VelDen.w;
    
    float glue = texture(FlowInkMap, Tex0).w;
    float FixBlk = texture(FixInkMap, Tex0).w;
    
    vec4 Disorder = texture(DisorderMap, Tex0);
    
    // Derive ad: Less advection for lower wf
    float ad = smoothstep(0.0, advect_p, wf);
    
    // Derive f0
    float f0_eq = A0 * wf - ad * (2.0 / 3.0) * dot(v, v);
    f0 = mix(f0, f0_eq, Omega);
    
    float GrainBlock = Disorder.x;
    float AlumBlock = Disorder.z;
    float block = Blk_1.x + dot(Blk_1.yz, vec2(GrainBlock, AlumBlock)) + dot(Blk_2.xy, vec2(glue, FixBlk));
    block = min(1.0, block);
    bool pinning = (wf == 0.0);
    
    float Pindisor = mix(Disorder.x, Disorder.w, smoothstep(0.0, toe_p, glue));
    float pin = Pin_w.x + dot(Pin_w.yz, vec2(FixBlk, Pindisor));
    
    pinning = (pinning) && (texture(VelDenMap, TexN_NE.xy).z < pin);
    pinning = (pinning) && (texture(VelDenMap, TexE_SE.xy).z < pin);
    pinning = (pinning) && (texture(VelDenMap, TexW_NW.xy).z < pin);
    pinning = (pinning) && (texture(VelDenMap, TexS_SW.xy).z < pin);
    
    pin *= Corn_mul;
    
    pinning = (pinning) && (texture(VelDenMap, TexN_NE.zw).z < pin);
    pinning = (pinning) && (texture(VelDenMap, TexE_SE.zw).z < pin);
    pinning = (pinning) && (texture(VelDenMap, TexW_NW.zw).z < pin);
    pinning = (pinning) && (texture(VelDenMap, TexS_SW.zw).z < pin);
    
    if (pinning)
        block = 1.0/0.0;
    
    fragColor = vec4(block, f0, wf, max(ws - seep, 0.0));
}