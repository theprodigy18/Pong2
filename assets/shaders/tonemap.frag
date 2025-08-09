#version 430 core 

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform sampler2D hdr;
layout (binding = 1) uniform sampler2D bloom;

void main()
{   
    vec3 hdrColor = texture(hdr, uv).rgb;
    vec3 bloomColor = texture(bloom, uv).rgb;
    
    vec3 combined = hdrColor + bloomColor;
    
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    
    combined *= 1.5;
    
    vec3 mapped = clamp((combined*(a*combined+b))/(combined*(c*combined+d)+e), 0.0, 1.0);
    
    outColor = vec4(mapped, 1.0);
}