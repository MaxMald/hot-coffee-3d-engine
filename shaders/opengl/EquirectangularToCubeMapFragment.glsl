#version 420 core

#include "commons/utilities.glsl"

layout(location = 0) in vec3 vLocalPosition;

layout(location = 0) out vec4 OutFragColor;

layout(binding = 0) uniform sampler2D uEquirectangularMap;

void main()
{
    vec2 uv = sampleSphericalMap(normalize(vLocalPosition));
    vec3 color = texture(uEquirectangularMap, uv).rgb;

    // HDRI image usually comes from linear space, so we need to apply tone
    // mapping and gamma correction    
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); // Apply gamma correction
    
    OutFragColor = vec4(color, 1.0);
}