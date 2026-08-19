#version 420 core

layout(std140, binding = 1) uniform CameraFrameBlock
{
  mat4 projection;
  mat4 view;
  vec3 cameraPosition;
  float cPadding0;
};

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

layout(std140, binding = 2) uniform LightBlock
{
  DirectionalLightData directionalLights[MAX_DIRECTIONAL_LIGHTS];
  OmniLightData omniLights[MAX_OMNI_LIGHTS];
  SpotLightData spotLights[MAX_SPOT_LIGHTS];
  int numDirectionalLights;
  int numOmniLights;
  int numSpotLights;
  int lPadding0;
};

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


layout(binding = 0) uniform sampler2D uPositionAndDepth;
layout(binding = 1) uniform sampler2D uNormalRoughness;
layout(binding = 2) uniform sampler2D uAlbedoAlpha;
layout(binding = 3) uniform sampler2D uMaterialParameters;
layout(binding = 4) uniform sampler2DArray uDirectionalShadowMaps;
layout(binding = 5) uniform sampler2DArray uSpotShadowMaps;

in vec2 vTexCoord;

out vec4 FragColor;

// utils

float saturate(float value)
{
  return clamp(value, 0.0, 1.0);
}

float calculateAttenuation(float distance, float range)
{
  // Quadratic attenuation that clamps to 0 at the light's range
  float attenuation = clamp(1.0 - distance / range, 0.001, 1.0);
  return attenuation * attenuation; // quadratic falloff
}

float linearizeDepth(float depth, float nearPlane, float farPlane)
{
  float z = depth * 2.0 - 1.0; // Back to NDC
  return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

// Lighting methods

vec4 calculateOmniLight(
  OmniLightData light, 
  vec4 diffuseColor,
  vec3 normal, 
  vec3 viewDir, 
  vec3 worldPos, 
  float specularStrength, 
  float shininess
)
{
  vec3 lightDir = normalize(light.position.xyz - worldPos);
  vec3 halfDir = normalize(lightDir + viewDir);

  float incidenceDiffuse = saturate(dot(normal, lightDir));
  float kD = incidenceDiffuse * 0.8;

  float incidenceSpecular = pow(saturate(dot(normal, halfDir)), shininess);
  float kS = incidenceSpecular * specularStrength * 0.2;

  float distance = length(light.position.xyz - worldPos);
  float attenuation = calculateAttenuation(distance, light.range);
  float attenuatedIntensity = light.intensity * attenuation;

  return vec4((kD * diffuseColor.rgb * light.color.rgb + kS) * attenuatedIntensity, 1.0);
}

float calculateDirectionalShadow(
  mat4 lightSpaceMatrix,
  vec3 fragPos,
  float bias,
  int shadowMapIndex
)
{
  vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0)
      return 0.0;

  if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
      return 0.0;

  // PCF for soft shadows
  // Sampling 9 neighboring texels in the shadow map

  float shadow = 0.0f;
  vec2 texelSize = 1.0 / textureSize(uDirectionalShadowMaps, 0).xy;
  for (int x = -1; x <= 1; ++x)
  {
      for (int y = -1; y <= 1; ++y)
      {
          float pcfClosestDepth = texture(uDirectionalShadowMaps, vec3(projCoords.xy + vec2(x, y) * texelSize, shadowMapIndex)).r;
          float currentDepth = projCoords.z;
          shadow += currentDepth - bias > pcfClosestDepth ? 1.0 : 0.0;
      }
  }

  // Average the shadow factor over the 9 samples
  shadow /= 9.0;

  return shadow;
}

vec4 calculateDirectionalLight(
  DirectionalLightData light,
  vec4 diffuseColor,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  float specularStrength,
  float shininess
)
{
  vec3 lightDir = normalize(-light.directionAndIntensity.xyz);
  vec3 halfDir = normalize(lightDir + viewDir);

  float incidenceDiffuse = saturate(dot(normal, lightDir));
  float kD = incidenceDiffuse * 0.8;

  float incidenceSpecular = pow(saturate(dot(normal, halfDir)), shininess);
  float kS = incidenceSpecular * specularStrength * 0.2;

  float shadowFactor = 1.0;
  if (light.shadowFrameDataIndex >= 0 && light.shadowFrameDataIndex < MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA)
  {
    DirectionalLightShadowFrameData shadowData = directionalLightShadowData[light.shadowFrameDataIndex];

    float cosNormalLight = dot(normal, lightDir);
    float bias = max(shadowData.shadowBias * (1.0 - cosNormalLight), 0.002);
    float shadow = calculateDirectionalShadow(
      shadowData.lightViewProjectionMatrix,
      worldPos,
      bias,
      shadowData.shadowMapIndex
    );

    shadowFactor = 1.0 - shadow * shadowData.shadowStrength;
  }

  float lightIntensity = light.directionAndIntensity.w;
  return vec4(((diffuseColor.rgb * kD * light.color.rgb) + kS) * lightIntensity * shadowFactor, 1.0);
}

float calculateSpotLightShadow(
  mat4 lightSpaceMatrix,
  vec3 fragPos,
  float bias,
  int shadowMapIndex,
  float projectionNearPlane,
  float projectionFarPlane
)
{
  vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0)
      return 0.0;

  if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
      return 0.0;

  // PCF for soft shadows
  // Sampling 9 neighboring texels in the shadow map

  float shadow = 0.0f;
  vec2 texelSize = 1.0 / textureSize(uSpotShadowMaps, 0).xy;
  for (int x = -1; x <= 1; ++x)
  {
    for (int y = -1; y <= 1; ++y)
    {
        float closestDepth = texture(uSpotShadowMaps, vec3(projCoords.xy + vec2(x, y) * texelSize, shadowMapIndex)).r;
        float currentDepth = projCoords.z;

        closestDepth = linearizeDepth(closestDepth, projectionNearPlane, projectionFarPlane);
        currentDepth = linearizeDepth(currentDepth, projectionNearPlane, projectionFarPlane);

        shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }
  }

  // Average the shadow factor over the 9 samples
  shadow /= 9.0;
  return shadow;
}

vec4 calculateSpotLight(
  SpotLightData light,
  vec4 diffuseColor,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  float specularStrength,
  float shininess
)
{
  vec3 lightDir = normalize(light.position.xyz - worldPos);
  vec3 halfDir = normalize(lightDir + viewDir);

  float incidenceDiffuse = saturate(dot(normal, lightDir));
  float kD = incidenceDiffuse * 0.8;

  float incidenceSpecular = pow(saturate(dot(normal, halfDir)), shininess);
  float kS = incidenceSpecular * specularStrength * 0.2;

  float distance = length(light.position.xyz - worldPos);
  float attenuation = calculateAttenuation(distance, light.range);
  float attenuatedIntensity = light.intensity * attenuation;

  float theta = dot(-light.direction.xyz, lightDir);
  if (theta > light.outerConeCos)
  {
    float epsilon = clamp(light.innerConeCos - light.outerConeCos, 0.001, 1.0);
    float spillLightIntensity = clamp((theta - light.outerConeCos) / epsilon, 0.0, 1.0);

    float shadowFactor = 1.0;
    if(light.shadowFrameDataIndex >= 0 && light.shadowFrameDataIndex < MAX_SPOT_LIGHTS_SHADOW_DATA)
    {
      SpotLightShadowFrameData shadowData = spotLightShadowData[light.shadowFrameDataIndex];

      float bias = max(shadowData.shadowBias * (1.0 - dot(normal, lightDir)), 0.002);
      float shadow = calculateSpotLightShadow(
        shadowData.lightViewProjectionMatrix,
        worldPos,
        bias,
        shadowData.shadowMapIndex,
        shadowData.projectionNearPlane,
        shadowData.projectionFarPlane
      );

      shadowFactor = 1.0 - shadow * shadowData.shadowStrength;
    }

    return vec4((kD * diffuseColor.rgb * light.color.rgb + kS) * attenuatedIntensity * spillLightIntensity * shadowFactor, 1.0);
  }
  else
  {
    return vec4(0.0, 0.0, 0.0, 1.0);
  }
}

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
  float shininess = 256.0 * (1.0 - roughness); // Convert roughness back to shininess

  vec4 albedoAlpha = texture(uAlbedoAlpha, vTexCoord);
  vec3 albedo = albedoAlpha.rgb;
  float alpha = albedoAlpha.a;
  if (alpha < 0.01)
    discard; // Transparent pixel

  vec4 materialParams = texture(uMaterialParameters, vTexCoord);
  float specularStrength = materialParams.r;
  
  vec3 viewDir = normalize(cameraPosition - worldPos);

  vec4 totalLighting = vec4(0.0, 0.0, 0.0, 1.0);
  for (int i = 0; i < numOmniLights; ++i)
  {
    totalLighting += calculateOmniLight(
      omniLights[i],
      vec4(albedo, 1.0),
      normal, 
      viewDir, 
      worldPos, 
      specularStrength, 
      shininess
    );
  }
    
  for (int i = 0; i < numDirectionalLights; ++i)
  {
    totalLighting += calculateDirectionalLight(
      directionalLights[i], 
      vec4(albedo, 1.0),
      normal, 
      viewDir, 
      worldPos, 
      specularStrength, 
      shininess
    );
  }
    
  for (int i = 0; i < numSpotLights; ++i)
  {
    totalLighting += calculateSpotLight(
      spotLights[i], 
      vec4(albedo, 1.0),
      normal, 
      viewDir, 
      worldPos, 
      specularStrength, 
      shininess
    );
  } 

  FragColor = vec4(totalLighting.rgb, alpha);
}