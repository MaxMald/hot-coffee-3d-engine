#version 420 core

#include "commons/camera.glsl"
#include "commons/lighting.glsl"

layout(binding = 0) uniform sampler2D uPositionAndDepth;
layout(binding = 1) uniform sampler2D uNormalRoughness;
layout(binding = 2) uniform sampler2D uAlbedoAlpha;
layout(binding = 3) uniform sampler2D uMaterialParameters;
layout(binding = 4) uniform sampler2D uSpecularColorAndShininess;

layout(location = 0) in vec2 vTexCoord;

layout(location = 0) out vec4 FragColor;

void main()
{
  vec4 positionAndDepth = texture(uPositionAndDepth, vTexCoord);
  vec3 worldPos = positionAndDepth.xyz;
  float depth = positionAndDepth.w;

  if (depth < 0.0)
    discard; // No geometry at this pixel

  vec4 normalRoughness = texture(uNormalRoughness, vTexCoord);
  vec3 normal = normalRoughness.xyz;
  float roughness = normalRoughness.w;

  vec4 specularColorAndShininess = texture(uSpecularColorAndShininess, vTexCoord);
  vec3 specularColor = specularColorAndShininess.rgb;
  float shininess = specularColorAndShininess.a * 256.0; // Convert shininess back from [0,1] range
  float specularStrength = texture(uMaterialParameters, vTexCoord).x;

  vec4 albedoSample = texture(uAlbedoAlpha, vTexCoord);
  float alpha = albedoSample.a;
  if (alpha < 0.01)
    discard; // Transparent pixel
  
  vec3 viewDir = normalize(cameraPosition - worldPos);
  
  vec4 ambientColor = albedoSample * 0.1; // Ambient light contribution
  vec4 lightedColor = calculateAllLightContribution(
    albedoSample,
    normal,
    viewDir,
    worldPos,
    specularColor,
    shininess
  );

  FragColor = vec4((ambientColor + lightedColor).rgb, alpha);
}