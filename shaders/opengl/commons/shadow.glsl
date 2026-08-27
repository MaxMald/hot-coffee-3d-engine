#version 420 core

#define MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA 4
#define MAX_SPOT_LIGHTS_SHADOW_DATA 8

struct DirectionalLightShadowFrameData
{
  mat4 lightViewProjectionMatrix;
  float shadowBias;
  float shadowStrength;
  int shadowMapIndex;
  int padding0;
};

struct SpotLightShadowFrameData
{
  mat4 lightViewProjectionMatrix;
  float shadowBias;
  float shadowStrength;
  float projectionNearPlane;
  float projectionFarPlane;
  int shadowMapIndex;
  int padding0;
  int padding1;
  int padding2;
};

layout(std140, binding = 3) uniform LightShadowBlock
{
  DirectionalLightShadowFrameData directionalLightShadowData[MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA];
  SpotLightShadowFrameData spotLightShadowData[MAX_SPOT_LIGHTS_SHADOW_DATA];
};

/**
* @brief Calculates the bias for shadow mapping based on the angle between the
* surface normal and the light direction. This helps to reduce shadow acne.
*
* @param normal The surface normal at the fragment.
* @param lightDir The direction from the surface toward the light source.
* @param shadowBias The base bias value to be adjusted based on the angle.
*
* @return The calculated bias value to be used in shadow mapping.
*/
float calculateBias(vec3 normal, vec3 lightDir, float shadowBias)
{
  float cosNormalLight = dot(normal, lightDir);
  return max(shadowBias * (1.0 - cosNormalLight), 0.002);
}

/**
* @brief Linearly interpolates the depth value from the non-linear depth buffer
* to a linear depth value based on the near and far planes of the camera's projection.
*
* @param depth The non-linear depth value from the depth buffer (range [0, 1]).
* @param nearPlane The near plane distance of the camera's projection.
* @param farPlane The far plane distance of the camera's projection.
*
* @return The linear depth value corresponding to the input depth.
*/
float linearizeDepth(float depth, float nearPlane, float farPlane)
{
  float z = depth * 2.0 - 1.0; // Back to NDC
  return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

/**
* @brief Computes the shadowed color contribution of a directional light.
*
* @param lightShadowIndex Index into the directional shadow data array.
* @param color The incoming light contribution color to be modulated by shadow.
* @param fragPos Fragment world position used to project into light space.
* @param normal Surface normal used to compute a bias that reduces acne.
* @param lightDir Direction from the surface toward the directional light.
*
* @return The original color multiplied by the shadow attenuation factor.
*/
vec4 calculateDirectionalShadowContribution(
  int lightShadowIndex,
  vec4 color,
  vec3 fragPos,
  vec3 normal,
  vec3 lightDir
)
{
  if (lightShadowIndex < 0 || lightShadowIndex >= MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA)
    return color;

  DirectionalLightShadowFrameData shadowData = directionalLightShadowData[lightShadowIndex];
  mat4 lightSpaceMatrix = shadowData.lightViewProjectionMatrix;

  // transform fragment position to light space
  vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0)
    return color;

  if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
    return color;

  
  // PCF for soft shadows
  // Sampling 9 neighboring texels in the shadow map

  float bias = calculateBias(normal, lightDir, shadowData.shadowBias);
  float shadow = 0.0f;
  int shadowMapIndex = shadowData.shadowMapIndex;
  vec2 texelSize = 1.0 / textureSize(uDirectionalShadowMaps, 0).xy;

  for (int x = -1; x <= 1; ++x)
  {
      for (int y = -1; y <= 1; ++y)
      {
          float pcfClosestDepth = texture(
            uDirectionalShadowMaps, 
            vec3(projCoords.xy + vec2(x, y) * texelSize, shadowMapIndex)
          ).r;

          float currentDepth = projCoords.z;
          shadow += currentDepth - bias > pcfClosestDepth ? 1.0 : 0.0;
      }
  }

  // Average the shadow factor over the 9 samples
  shadow /= 9.0;

  return color * (1.0 - shadow * shadowData.shadowStrength);
}

/**
* @brief Computes the shadowed color contribution of a spot light.
*
* @param lightShadowIndex Index into the spot shadow data array.
* @param color The incoming light contribution color to be modulated by shadow.
* @param fragPos Fragment world position used to project into light space.
* @param normal Surface normal used to compute a bias that reduces acne.
* @param lightDir Direction from the surface toward the spot light.
*
* @return The original color multiplied by the shadow attenuation factor.
*/
vec4 calculateSpotLightShadowContribution(
  int lightShadowIndex,
  vec4 color,
  vec3 fragPos,
  vec3 normal,
  vec3 lightDir
)
{
  if (lightShadowIndex < 0 || lightShadowIndex >= MAX_SPOT_LIGHTS_SHADOW_DATA)
    return color;

  SpotLightShadowFrameData shadowData = spotLightShadowData[lightShadowIndex];
  mat4 lightSpaceMatrix = shadowData.lightViewProjectionMatrix;

  // transform fragment position to light space
  vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0)
    return color;

  if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
    return color;

  // PCF for soft shadows
  // Sampling 9 neighboring texels in the shadow map

  float bias = calculateBias(normal, lightDir, shadowData.shadowBias);
  float projectionNearPlane = shadowData.projectionNearPlane;
  float projectionFarPlane = shadowData.projectionFarPlane;
  float shadow = 0.0;
  int shadowMapIndex = shadowData.shadowMapIndex;
  vec2 texelSize = 1.0 / textureSize(uSpotShadowMaps, 0).xy;

  for (int x = -1; x <= 1; ++x)
  {
    for (int y = -1; y <= 1; ++y)
    {
        float closestDepth = texture(
          uSpotShadowMaps, 
          vec3(projCoords.xy + vec2(x, y) * texelSize, shadowMapIndex)
        ).r;

        float currentDepth = projCoords.z;

        closestDepth = linearizeDepth(closestDepth, projectionNearPlane, projectionFarPlane);
        currentDepth = linearizeDepth(currentDepth, projectionNearPlane, projectionFarPlane);

        shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }
  }

  // Average the shadow factor over the 9 samples
  shadow /= 9.0;

  return color * (1.0 - shadow * shadowData.shadowStrength);
}