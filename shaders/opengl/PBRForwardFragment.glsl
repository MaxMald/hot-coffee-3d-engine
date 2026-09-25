#version 420 core

#include "commons/camera.glsl"
#include "commons/sceneBlock.glsl"
#include "commons/lighting.glsl"
#include "commons/materialPBRBlock.glsl"

layout(location = 0) in vec2 vTexCoord;
layout(location = 1) in vec3 vWorldPos;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec4 vColor;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D uAlbedo;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uORM;

void main()
{
  vec4 albedoTex = texture(uAlbedo, vTexCoord);
 if (isAlphaLessThanCutoff(albedoTex, uAlphaCutoff))
    discard;

  vec3 N = normalize(vNormal);
  vec3 T = normalize(vTangent - dot(vTangent, N) * N);
  vec3 B = normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  vec3 normalTS = texture(uNormalMap, vTexCoord).xyz * 2.0 - 1.0;
  vec3 normalWS = normalize(TBN * normalTS);
  vec3 viewDir = normalize(cameraPosition - vWorldPos);  

  vec4 ormiorSample = texture(uORM, vTexCoord);
  float occlusion = ormiorSample.x;
  float roughness = ormiorSample.y;
  float metallic = ormiorSample.z;

  vec4 baseColor = uBaseColor * vColor * albedoTex;
  vec4 ambientColor = baseColor 
    * uSceneAmbientColor
    * uSceneAmbientIntensity 
    * occlusion;
  
  vec4 oRadiance = evaluateOutgoingRadiance(
    baseColor,
    normalWS,
    viewDir,
    vWorldPos,
    roughness,
    metallic,
    uIOR
  );

  FragColor = vec4((ambientColor + oRadiance).rgb, albedoTex.a);
}