#version 420 core

#include "commons/camera.glsl"
#include "commons/lighting.glsl"
#include "commons/materialHair.glsl"

in vec2 vTexCoord;
in vec3 vWorldPos;
in vec3 vNormal;
in vec3 vTangent;
in vec4 vColor;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D uAlbedo;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 5) uniform sampler2DArray uDirectionalShadowMaps;
layout(binding = 6) uniform sampler2DArray uSpotShadowMaps;

void main()
{ 
  vec4 albedoTex = texture(uAlbedo, vTexCoord);
  if (!isAlphaLessThanCutoff(albedoTex, uAlphaCutoff))
    discard; // Opaque hair strands are discarded

  vec3 N = normalize(vNormal);
  vec3 T = normalize(vTangent - dot(vTangent, N) * N);
  vec3 B = normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  vec3 normalTS = texture(uNormalMap, vTexCoord).xyz * 2.0 - 1.0;
  vec3 normalWS = normalize(TBN * normalTS);
  vec3 viewDir = normalize(cameraPosition - vWorldPos);

  vec4 baseColor = albedoTex * vColor * uColor * 0.1; // Base color with ambient light contribution
  vec4 lightedColor = calculateAllLightContribution(
    baseColor, 
    normalWS, 
    viewDir, 
    vWorldPos, 
    vec3(0.0, 0.0, 0.0), // No specular contribution in this pass
    1.0
  );

  FragColor = vec4(lightedColor.rgb, albedoTex.a);
}