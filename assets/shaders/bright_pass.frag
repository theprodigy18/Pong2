#version 430 core 

layout (location = 0) in vec2 uv;
layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform sampler2D hdr;


void main()
{   
    vec3 hdrColor = texture(hdr, uv).rgb;
    
    float brightness = dot(hdrColor, vec3(0.2126, 0.7152, 0.0722));
    
    // Scaling factor.
    float bloomFactor = max(0.0, brightness - 0.5) * 2.0;
    
    outColor = vec4(hdrColor * bloomFactor, 1.0);
}