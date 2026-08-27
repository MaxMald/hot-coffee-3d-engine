#version 420 core

#include "commons/camera.glsl"
#include "commons/lighting.glsl"
#include "commons/materialBlinnPhong.glsl"

in vec2 vTexCoord;
in vec3 vWorldPos;
in vec3 vNormal;
in vec3 vTangent;
in vec4 vColor;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D uAlbedo;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uSpecularMap;

void main()
{
  vec4 albedoTex = texture(uAlbedo, vTexCoord);
  if (uAlphaCutoff > 0.0 && albedoTex.a < uAlphaCutoff)
    discard;

  vec3 N = normalize(vNormal);
  vec3 T = normalize(vTangent - dot(vTangent, N) * N);
  vec3 B = normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  vec3 normalTS = texture(uNormalMap, vTexCoord).xyz * 2.0 - 1.0;
  vec3 normalWS = normalize(TBN * normalTS);
  vec3 viewDir = normalize(cameraPosition - vWorldPos);  

  vec4 specularSample = texture(uSpecularMap, vTexCoord);
  vec3 specularColor = specularSample.rgb;
  float shininess = uShininess * specularSample.a;

  vec4 baseColor = uColor * vColor * albedoTex * 0.1; // Ambient light contribution
    
  vec4 lightedColor = calculateAllLightContribution(
    baseColor,
    normalWS,
    viewDir,
    vWorldPos,
    specularColor,
    shininess
  );

  FragColor = vec4(lightedColor.rgb, albedoTex.a);
}