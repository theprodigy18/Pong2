#version 430 core 

layout (location = 0) in vec2 uv;
layout (location = 0) out vec4 outColor;

layout (binding = 0) uniform sampler2D brightTexture;

void main()
{   
    vec2 texelSize = 1.0 / textureSize(brightTexture, 0);
    vec3 result = vec3(0.0);
    
    // Large radius.
    int radius = 30; 
    float totalWeight = 0.0;
    
    for(int x = -radius; x <= radius; ++x) {
        for(int y = -radius; y <= radius; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            vec3 samplea = texture(brightTexture, uv + offset).rgb;
            
            // Smooth falloff.
            float distance = length(vec2(float(x), float(y))) / float(radius);
            float weight = 1.0 - smoothstep(0.0, 1.0, distance);
            weight = weight * weight; // Quadratic falloff.
            
            result += samplea * weight;
            totalWeight += weight;
        }
    }
    
    result /= totalWeight;
    outColor = vec4(result * 1.5, 1.0); // Boost brightness.
}