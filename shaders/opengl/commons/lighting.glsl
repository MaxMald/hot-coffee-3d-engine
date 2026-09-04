#include "utilities.glsl"
#include "shadow.glsl"

#define MAX_OMNI_LIGHTS 16
#define MAX_SPOT_LIGHTS 8
#define MAX_DIRECTIONAL_LIGHTS 4

struct SpotLightData
{
  vec4 position;
  vec4 direction;
  vec4 color;
  float range;
  float innerConeCos;
  float intensity;
  float outerConeCos;
  int shadowFrameDataIndex;
  int padding0;
  int padding1;
  int padding2;
};

struct OmniLightData
{
  vec4 position;
  vec4 color;
  float range;
  float intensity;
  float padding0;
  float padding1;
};

struct DirectionalLightData
{
  vec4 directionAndIntensity;
  vec4 color;
  int  shadowFrameDataIndex;
  int  padding0;
  int  padding1;
  int  padding2;
};

layout(std140, binding = 1) uniform LightBlock
{
  DirectionalLightData directionalLights[MAX_DIRECTIONAL_LIGHTS];
  OmniLightData omniLights[MAX_OMNI_LIGHTS];
  SpotLightData spotLights[MAX_SPOT_LIGHTS];
  int numDirectionalLights;
  int numOmniLights;
  int numSpotLights;
  int lPadding0;
};

/**
* @brief Calculates the attenuation factor for a light based on its distance and
* range. The attenuation is quadratic and clamps to 0 at the light's range.
*
* @param distance The distance from the light to the point being lit.
* @param range The effective range of the light.
*
* @return The attenuation factor, which is a value between 0.001 and 1.0.
*/
float calculateAttenuation(float distance, float range)
{
  // Quadratic attenuation that clamps to 0 at the light's range
  float attenuation = clamp(1.0 - distance / range, 0.001, 1.0);
  return attenuation * attenuation; // quadratic falloff
}

/**
* @brief Calculates the contribution of a directional light to the final color
* of a fragment.
*
* @param lightIndex The index of the directional light in the directionalLights array.
* @param diffuseColor The base diffuse color of the fragment.
* @param normal The normal vector at the fragment's surface.
* @param viewDir The direction from the fragment to the camera/viewer.
* @param worldPos The world position of the fragment.
* @param specularColor The base specular color of the fragment.
* @param shininess The shininess factor for specular reflection.
*
* @return The final color contribution of the directional light as a vec4.
*/
vec4 calculateDirectionalLightContribution(
  int lightIndex,
  vec4 diffuseColor,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  vec3 specularColor,
  float shininess
)
{
  if (lightIndex < 0 || lightIndex >= MAX_DIRECTIONAL_LIGHTS)
  {
    return vec4(0.0, 0.0, 0.0, 1.0);
  }

  DirectionalLightData light = directionalLights[lightIndex];

  vec3 lightDir = normalize(-light.directionAndIntensity.xyz);
  vec3 halfDir = normalize(lightDir + viewDir);

  float incidenceDiffuse = saturate(dot(normal, lightDir));
  float kD = incidenceDiffuse * 0.8;

  float incidenceSpecular = pow(saturate(dot(normal, halfDir)), shininess);
  float kS = incidenceSpecular * 0.2;

  float lightIntensity = light.directionAndIntensity.w;
  vec4 lightedColor = vec4(((diffuseColor.rgb * kD * light.color.rgb) + (kS * specularColor)) * lightIntensity , 1.0);

  lightedColor = calculateDirectionalShadowContribution(
    light.shadowFrameDataIndex, 
    lightedColor, 
    worldPos, 
    normal, 
    lightDir
  );

  return lightedColor;
}

/**
* @brief Calculates the contribution of a spot light to the final color
* of a fragment.
*
* @param lightIndex The index of the spot light in the spotLights array.
* @param diffuseColor The base diffuse color of the fragment.
* @param normal The normal vector at the fragment's surface.
* @param viewDir The direction from the fragment to the camera/viewer.
* @param worldPos The world position of the fragment.
* @param specularColor The base specular color of the fragment.
* @param shininess The shininess factor for specular reflection.
*
* @return The final color contribution of the spot light as a vec4.
*/
vec4 calculateSpotLightContribution(
  int lightIndex,
  vec4 diffuseColor,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  vec3 specularColor,
  float shininess
)
{
  if (lightIndex < 0 || lightIndex >= MAX_SPOT_LIGHTS)
  {
    return vec4(0.0, 0.0, 0.0, 1.0);
  }

  SpotLightData light = spotLights[lightIndex];
  vec3 lightDir = normalize(light.position.xyz - worldPos);
  
  float theta = dot(-light.direction.xyz, lightDir);
  if (theta <= light.outerConeCos)
  {
    return vec4(0.0, 0.0, 0.0, 1.0);
  }

  vec3 halfDir = normalize(lightDir + viewDir);

  float incidenceDiffuse = saturate(dot(normal, lightDir));
  float kD = incidenceDiffuse * 0.8;

  float incidenceSpecular = pow(saturate(dot(normal, halfDir)), shininess);
  float kS = incidenceSpecular * 0.2;

  float distance = length(light.position.xyz - worldPos);
  float attenuation = calculateAttenuation(distance, light.range);
  float attenuatedIntensity = light.intensity * attenuation;

  float epsilon = clamp(light.innerConeCos - light.outerConeCos, 0.001, 1.0);
  float spillLightIntensity = clamp((theta - light.outerConeCos) / epsilon, 0.0, 1.0);

  vec4 lightedColor = vec4(((kD * diffuseColor.rgb * light.color.rgb) + (kS * specularColor)) * attenuatedIntensity * spillLightIntensity, 1.0);

  lightedColor = calculateSpotLightShadowContribution(
    light.shadowFrameDataIndex, 
    lightedColor, 
    worldPos, 
    normal, 
    lightDir
  );

  return lightedColor;
}

/**
* @brief Calculates the contribution of an omni-directional light to the final color
* of a fragment.
*
* @param lightIndex The index of the omni light in the omniLights array.
* @param diffuseColor The base diffuse color of the fragment.
* @param normal The normal vector at the fragment's surface.
* @param viewDir The direction from the fragment to the camera/viewer.
* @param worldPos The world position of the fragment.
* @param specularColor The base specular color of the fragment.
* @param shininess The shininess factor for specular reflection.
*
* @return The final color contribution of the omni light as a vec4.
*/
vec4 calculateOmniLightContribution(
  int lightIndex, 
  vec4 diffuseColor,
  vec3 normal, 
  vec3 viewDir, 
  vec3 worldPos, 
  vec3 specularColor,
  float shininess
)
{
  if (lightIndex < 0 || lightIndex >= MAX_OMNI_LIGHTS)
  {
    return vec4(0.0, 0.0, 0.0, 1.0);
  }

  OmniLightData light = omniLights[lightIndex];
  vec3 lightDir = normalize(light.position.xyz - worldPos);
  vec3 halfDir = normalize(lightDir + viewDir);

  float incidenceDiffuse = saturate(dot(normal, lightDir));
  float kD = incidenceDiffuse * 0.8;

  float incidenceSpecular = pow(saturate(dot(normal, halfDir)), shininess);
  float kS = incidenceSpecular  * 0.2;

  float distance = length(light.position.xyz - worldPos);
  float attenuation = calculateAttenuation(distance, light.range);
  float attenuatedIntensity = light.intensity * attenuation;

  return vec4(((kD * diffuseColor.rgb * light.color.rgb) + (kS * specularColor)) * attenuatedIntensity, 1.0);
}

/**
* @brief Calculates the total light contribution from all omni, directional, and
* spot lights.
*
* @param diffuseColor The base diffuse color of the fragment.
* @param normal The normal vector at the fragment's surface.
* @param viewDir The direction from the fragment to the camera/viewer.
* @param worldPos The world position of the fragment.
* @param specularColor The base specular color of the fragment.
* @param shininess The shininess factor for specular reflection.
*
* @return The final color contribution from all lights as a vec4.
*/
vec4 calculateAllLightContribution(
  vec4 diffuseColor,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  vec3 specularColor,
  float shininess
)
{
  vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);
  for (int i = 0; i < numOmniLights; ++i)
  {
    finalColor += calculateOmniLightContribution(
      i,
      diffuseColor,
      normal, 
      viewDir, 
      worldPos, 
      specularColor,
      shininess
    );
  }

  for (int i = 0; i < numDirectionalLights; ++i)
  {
    finalColor += calculateDirectionalLightContribution(
      i, 
      diffuseColor,
      normal, 
      viewDir, 
      worldPos, 
      specularColor,
      shininess
    );
  }

  for (int i = 0; i < numSpotLights; ++i)
  {
    finalColor += calculateSpotLightContribution(
      i, 
      diffuseColor,
      normal, 
      viewDir, 
      worldPos, 
      specularColor,
      shininess
    );
  }

  return finalColor;
}
