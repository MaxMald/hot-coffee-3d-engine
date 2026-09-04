#version 420 core

#include "commons/utilities.glsl"
#include "commons/materialBlinnPhong.glsl"

layout(location = 0) in vec2 vTexCoord;
layout(location = 1) in vec3 vWorldPos;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec4 vColor;

layout(location = 0) out vec4 OutPositionAndDepth;
layout(location = 1) out vec4 OutNormalRoughness;
layout(location = 2) out vec4 OutAlbedoAlpha;
layout(location = 3) out vec4 OutMaterialParameters;
layout(location = 4) out vec4 OutSpecularColorAndShininess;

layout(binding = 0) uniform sampler2D uAlbedo;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uSpecularMap;

void main()
{
  vec4 albedoTex = texture(uAlbedo, vTexCoord);
  if (isAlphaLessThanCutoff(albedoTex, uAlphaCutoff))
    discard;

  vec4 baseColor = uColor * vColor * albedoTex;

  vec3 N = normalize(vNormal);
  vec3 T = normalize(vTangent - dot(vTangent, N) * N);
  vec3 B = normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);
  vec3 normalTS = texture(uNormalMap, vTexCoord).xyz * 2.0 - 1.0;
  vec3 normalWS = normalize(TBN * normalTS);

  vec4 specularSample = texture(uSpecularMap, vTexCoord);
  vec3 specularColor = specularSample.rgb * specularSample.a;
  float shininess = uShininess;

  // Convert shininess to roughness, currently we are using shininess in the
  // deferred lighting pass, but we can switch to roughness if needed.
  float roughness = 1.0 - (shininess / 256.0); 

  OutPositionAndDepth = vec4(vWorldPos, gl_FragCoord.z);
  OutNormalRoughness = vec4(normalWS, roughness);
  OutAlbedoAlpha = baseColor;
  OutMaterialParameters = vec4(0.0, 0.0, 0.0, 1.0);
  OutSpecularColorAndShininess = vec4(specularColor, shininess / 256.0);
}