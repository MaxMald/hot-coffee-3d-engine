#version 420 core

#include "commons/utilities.glsl"
#include "commons/cubeMapGenerator.glsl"

layout(location = 0) in vec3 vLocalPosition;

layout(location = 0) out vec4 OutFragColor;

layout(binding = 0) uniform samplerCube uInputCubeMap;

float distributionGGX(vec3 N, vec3 H, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float denom = NdotH2 * (a2 - 1.0) + 1.0;
  denom = PI * denom * denom;
  return a2 / max(denom, EPSILON);
}

vec3 importanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

/**
 * Computes the appropriate mip level for a given sample based on the GGX
   distribution.
 *
 * @param N The surface normal.
 * @param H The half-vector between view and light directions.
 * @param V The view direction.
 * @param roughness The surface roughness.
 * @param sourceResolution The resolution of the source environment map.
 * @param sampleCount The number of samples used for importance sampling.
 * @return The computed mip level.
 */
float computeMipLevel(
    vec3 N, vec3 H, vec3 V,
    float roughness,
    float sourceResolution, 
    uint sampleCount)
{
    float D = distributionGGX(N, H, roughness);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);
    
    float pdf = (D * NdotH) / max(4.0 * HdotV, EPSILON);
    float saTexel = 4.0 * PI / (6.0 * sourceResolution * sourceResolution);
    float saSample = 1.0 / max(float(sampleCount) * pdf, EPSILON);

    return roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
}

void main()
{
    vec3 normal = normalize(vLocalPosition); 
    vec3 reflection = normal;
    vec3 view = reflection;

    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3(0.0);

    for(uint i = 0u; i < ucmgSampleCount; ++i)
    {
        vec2 Xi = hammersley(i, ucmgSampleCount);
        vec3 H = importanceSampleGGX(Xi, normal, ucmgRoughness);
        vec3 L = normalize(2.0 * dot(view, H) * H - view);

        // Instead of using the base mip level, compute the appropriate mip
        // level for the current sample based on the GGX distribution. This
        // trick help to reduce the sampling artifacts like bright spots and
        // noise.
        //
        // More info: https://learnopengl.com/PBR/IBL/Specular-IBL
        int sourceResolution = textureSize(uInputCubeMap, 0).x;
        float mipLevel = computeMipLevel(
            normal, H, view, 
            ucmgRoughness, 
            float(sourceResolution), 
            ucmgSampleCount
        );

        float NdotL = max(dot(normal, L), 0.0);
        if(NdotL > 0.0)
        {
            prefilteredColor += textureLod(uInputCubeMap, L, mipLevel).rgb * NdotL;
            totalWeight += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;
    OutFragColor = vec4(prefilteredColor, 1.0);
}