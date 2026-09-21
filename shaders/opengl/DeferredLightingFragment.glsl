#version 420 core

#include "commons/camera.glsl"
#include "commons/lighting.glsl"

layout(binding = 0) uniform sampler2D uPositionAndDepth;
layout(binding = 1) uniform sampler2D uNormal; 
layout(binding = 2) uniform sampler2D uAlbedoAlpha;
layout(binding = 3) uniform sampler2D uORMIOR;

layout(location = 0) in vec2 vTexCoord;

layout(location = 0) out vec4 FragColor;

void main()
{
  vec4 positionAndDepth = texture(uPositionAndDepth, vTexCoord);
  vec3 worldPos = positionAndDepth.xyz;
  float depth = positionAndDepth.w;

  if (depth < 0.0)
    discard; // No geometry at this pixel

  vec4 albedoSample = texture(uAlbedoAlpha, vTexCoord);
  float alpha = albedoSample.a;
  if (alpha < 0.01)
    discard; // Transparent pixel
  
  vec4 baseColor = vec4(albedoSample.rgb, 1.0);
  vec4 ambientColor = vec4(baseColor.rgb * 0.1, 1.0); // Ambient light contribution

  vec3 normal = texture(uNormal, vTexCoord).xyz;
  vec3 viewDir = normalize(cameraPosition - worldPos);
  
  vec4 ormiorSample = texture(uORMIOR, vTexCoord);
  float occlusion = ormiorSample.x;
  float roughness = ormiorSample.y;
  float metallic = ormiorSample.z;
  float ior = ormiorSample.w * MAXIMUM_IOR;
  
  /*
  vec4 lightedColor = calculateAllLightContribution(
    albedoSample,
    normal,
    viewDir,
    worldPos,
    specularColor,
    shininess
  );*/
  vec4 lightedColor = vec4(0.0); // TODO lighting for deferred shading

  FragColor = vec4((ambientColor + lightedColor).rgb, alpha);
}