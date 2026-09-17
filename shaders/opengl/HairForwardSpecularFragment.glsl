#version 420 core

#include "commons/camera.glsl"
#include "commons/lighting.glsl"
#include "commons/materialHair.glsl"

layout(location = 0) in vec2 vTexCoord;
layout(location = 1) in vec3 vWorldPos;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec4 vColor;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D uAlbedo;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uSpecularMap;

// Anisotropic Rendering: Kajiya Kay Hair Rendering
// Reference: https://zhuanlan.zhihu.com/p/363829203

vec3 shiftTangent(vec3 T, vec3 N, float shift)
{
  return normalize(T + shift * N);
}

float hairStrand(vec3 T, vec3 V, vec3 L, float specularStrength, float shininess, float specularWidth)
{
  vec3 H = normalize(V + L);

  float HdotT = dot(T, H);
  float sinTH = sqrt(max(1 - HdotT * HdotT, 0.0));
  float dirAtten = smoothstep(-max(specularWidth, 1e-4), 0, HdotT);

  return dirAtten * saturate(pow(sinTH, shininess)) * specularStrength;
}

vec4 calculateKajiyaKaySpecular(
  vec4 lightColor0,
  vec4 primaryColor, 
  float primaryShift,
  vec4 secondaryColor, 
  float secondaryShift,
  vec3 N, vec3 T, vec3 VDir, vec3 LDir, 
  float specularStrength,
  float shininess,
  float specularWidth
  /*, vec2 uv */ // uncomment when using specular shift texture
)
{
  // Uncomment the following line when using specular shift texture
  // float shiftTex = tex2D(_SpecularShift, uv) - 0.5;
  float shiftTex = 0.0; // Remove this line when using specular shift texture

  vec3 t1 = shiftTangent(T, N, primaryShift + shiftTex);
  vec3 t2 = shiftTangent(T, N, secondaryShift + shiftTex);

  vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
  specular += primaryColor * hairStrand(t1, VDir, LDir, specularStrength, shininess, specularWidth);
  specular += secondaryColor * hairStrand(t2, VDir, LDir, specularStrength, shininess, specularWidth);

  // Kajiya-Kay's tangent-based term ignores N, so mask out highlights where the
  // light can't actually reach the surface
  float NdotL = saturate(dot(N, LDir));
  specular *= NdotL;

  return specular;
}

vec4 computeSpecular(
  vec4 specularColor,
  vec4 primaryColor,
  float primaryShift,
  vec4 secondaryColor,
  float secondaryShift,
  vec3 N, vec3 T, vec3 VDir,
  float specularStrength,
  float shininess,
  float specularWidth
  /*, vec2 uv */ // uncomment when using specular shift texture
)
{
  vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);

  for (int i = 0; i < numDirectionalLights; ++i)
  {
    DirectionalLightData light = directionalLights[i];
    vec3 LDir = normalize(-light.directionAndIntensity.xyz);
    float lightIntensity = light.directionAndIntensity.w;

    vec4 kaySpecular = calculateKajiyaKaySpecular(
      light.color,
      primaryColor, primaryShift,
      secondaryColor, secondaryShift,
      N, T, VDir, LDir,
      specularStrength,
      shininess,
      specularWidth
      /*, uv */ // uncomment when using specular shift texture
    );

    // Apply light intensity and color to the Kajiya-Kay specular term
    kaySpecular *= lightIntensity * specularColor * light.color;

    // Apply shadowing to the Kajiya-Kay specular term
    kaySpecular = calculateDirectionalShadowContribution(
      light.shadowFrameDataIndex, 
      kaySpecular, 
      vWorldPos, 
      N, 
      LDir
    );

    // Accumulate the Kajiya-Kay specular contribution from this directional light
    specular += kaySpecular;
  }

  for (int i = 0; i < numOmniLights; ++i)
  {
    OmniLightData light = omniLights[i];
    vec3 LDir = normalize(light.position.xyz - vWorldPos);

    // Ominlight attenuation based on distance and range
    float distance = length(light.position.xyz - vWorldPos);
    float attenuation = calculateAttenuation(distance, light.range);
    float lightIntensity = light.intensity;

    vec4 kaySpecular = calculateKajiyaKaySpecular(
      light.color,
      primaryColor, primaryShift,
      secondaryColor, secondaryShift,
      N, T, VDir, LDir,
      specularStrength,
      shininess,
      specularWidth
      /*, uv */ // uncomment when using specular shift texture
    );

    // Apply light intensity and color to the Kajiya-Kay specular term
    kaySpecular *= lightIntensity * specularColor * light.color * attenuation;

    // Accumulate the Kajiya-Kay specular contribution from this omni light
    specular += kaySpecular;
  }

  for (int i = 0; i < numSpotLights; ++i)
  {
    SpotLightData light = spotLights[i];
    vec3 LDir = normalize(light.position.xyz - vWorldPos);

    // Spot light attenuation based on distance and range
    float distance = length(light.position.xyz - vWorldPos);
    float attenuation = calculateAttenuation(distance, light.range);
    float lightIntensity = light.intensity;

    vec4 kaySpecular = calculateKajiyaKaySpecular(
      light.color,
      primaryColor, primaryShift,
      secondaryColor, secondaryShift,
      N, T, VDir, LDir,
      specularStrength,
      shininess,
      specularWidth
      /*, uv */ // uncomment when using specular shift texture
    );

    // Apply shadowing to the Kajiya-Kay specular term
    kaySpecular = calculateSpotLightShadowContribution(
      light.shadowFrameDataIndex, 
      kaySpecular, 
      vWorldPos, 
      N, 
      LDir
    );

    // Apply light intensity, color, attenuation to the Kajiya-Kay specular term
    kaySpecular *= attenuation * specularColor * light.color * lightIntensity;

    // Accumulate the Kajiya-Kay specular contribution from this spot light
    specular += kaySpecular;
  }

  return specular;
}

void main()
{
  vec4 albedoTex = texture(uAlbedo, vTexCoord);
  if (isAlphaLessThanCutoff(albedoTex, uAlphaCutoff))
    discard; // Semi-transparent hair strands are discarded

  vec3 N = normalize(vNormal);
  vec3 T = normalize(vTangent - dot(vTangent, N) * N);
  vec3 B = normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  vec3 normalTS = texture(uNormalMap, vTexCoord).xyz * 2.0 - 1.0;
  vec3 normalWS = normalize(TBN * normalTS);
  vec3 viewDir = normalize(cameraPosition - vWorldPos);

  vec3 tangentWS = normalize(T - dot(T, normalWS) * normalWS);

  vec4 specularSample = texture(uSpecularMap, vTexCoord);
  vec3 specularColor = specularSample.rgb * specularSample.a;
  float shininess = uShininess;

  vec4 specular = computeSpecular(
    vec4(specularColor, 1.0),
    uPrimaryColor,
    uPrimaryShift,
    uSecondaryColor,
    uSecondaryShift,
    normalWS,
    tangentWS,
    viewDir,
    uSpecularStrength,
    shininess,
    uSpecularWidth
  );

  // specular color should be blended additively in the pipeline
  FragColor = specular;
}