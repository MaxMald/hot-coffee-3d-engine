#version 420 core

#include "commons/utilities.glsl"
#include "commons/cubeMapGenerator.glsl"

layout(location = 0) in vec3 vLocalPosition;

layout(location = 0) out vec4 OutFragColor;

layout(binding = 0) uniform samplerCube uInputCubeMap;

void main()
{
    vec3 normal = normalize(vLocalPosition); 
    
    // Build the orthonormal basis around the normal.
    vec3 referenceUp = abs(normal.y) < 0.999 
        ? vec3(0.0, 1.0, 0.0) 
        : vec3(1.0, 0.0, 0.0);

    vec3 right = normalize(cross(referenceUp, normal));
    vec3 up = cross(normal, right);

    // Set up sampling parameters for the spherical integration over the
    // hemisphere around the normal.
    float sampleDelta = ucmgSampleDelta;
    float countSamples = 0.0;
    float maxPhi = 2.0 * PI;
    float maxTheta = 0.5 * PI;

    vec3 irradiance = vec3(0.0);
    for(float phi = 0.0; phi < maxPhi; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < maxTheta; theta += sampleDelta)
        {
            // Spherical to Cartesian coordinates
            vec3 tangentSampleDirection = vec3(
                sin(theta) * cos(phi),
                sin(theta) * sin(phi),
                cos(theta)
            );

            // Transform the sample direction from local space to world space
            vec3 sampleDirection = tangentSampleDirection.x * right
                                    + tangentSampleDirection.y * up
                                    + tangentSampleDirection.z * normal;
            
            irradiance += textureLod(uInputCubeMap, sampleDirection, ucmgSourceMipLevel).rgb
                          * cos(theta)    // Because theta is measured relative to N (normal), this is the Lambertian incident-angle factor.
                          * sin(theta);   // This comes from the spherical-coordinate solid-angle differential. It compensates for the varying area represented by each angular sample.
            
            countSamples += 1.0;
        }
    }
    
    // Store normalied irradiance: E / PI
    irradiance *= PI / max(countSamples, EPSILON);
    OutFragColor = vec4(irradiance, 1.0);
}