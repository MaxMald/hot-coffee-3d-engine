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

// Shadow Data for lights

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

in vec2 vTexCoord;
in vec3 vWorldPos;
in vec3 vNormal;
in vec3 vTangent;
in vec4 vColor;

out vec4 FragColor;

uniform vec4 uPrimaryColor;
uniform vec4 uSecondaryColor;
uniform float uPrimaryShift;
uniform float uSecondaryShift;
uniform float uSpecularWidth;
uniform float uSpecularStrength;
uniform float uAlphaCutoff;
uniform float uShininess;

layout(binding = 1) uniform sampler2D uAlbedo;
layout(binding = 2) uniform sampler2D uNormalMap;
layout(binding = 3) uniform sampler2D uSpecularMap;
layout(binding = 5) uniform sampler2DArray uDirectionalShadowMaps;
layout(binding = 6) uniform sampler2DArray uSpotShadowMaps;

/**
 * @brief Performs alpha testing on a color against a specified alpha cutoff
 * value. If the alpha component of the color is less than this value, the
 * function returns true (indicating that the fragment should be discarded).
 *
 * If the alpha cutoff is less than or equal to zero, the function will always
 * return false, meaning that no fragments will be discarded based on alpha
 * testing.
 * 
 * @param color The color to test, which includes an alpha component.
 * @param alphaCutoff The alpha cutoff value to compare against.
 * 
 * @return true if the alpha component of the color is less than the alpha
 * cutoff value.
 */
bool isAlphaLessThanCutoff(vec4 color, float alphaCutoff)
{
  return alphaCutoff > 0.0 && color.a < alphaCutoff;
}

float saturate(float x)
{
  return clamp(x, 0.0, 1.0);
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

vec4 getSpecular(
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

  // Kajiya-Kay's tangent-based term ignores N, so mask out highlights where the light can't actually reach the surface
  float NdotL = saturate(dot(N, LDir));
  specular *= NdotL;

  return specular;
}

float calculateDirectionalShadow(
  int shadowFrameDataIndex,
  vec3 N, vec3 LDir, vec3 fragWorldPos
)
{
  if (shadowFrameDataIndex < 0 || shadowFrameDataIndex >= MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA)
  {
    return 1.0; // No shadow if the index is invalid
  }

  DirectionalLightShadowFrameData shadowData = directionalLightShadowData[shadowFrameDataIndex];

  float cosNormalLight = dot(N, LDir);
  float bias = max(shadowData.shadowBias * (1.0 - cosNormalLight), 0.002);
  mat4 lightSpaceMatrix = shadowData.lightViewProjectionMatrix;

  vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragWorldPos, 1.0);
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0)
    return 1.0; // No shadow if fragment is outside the light's frustum

  if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
    return 1.0; // No shadow if fragment is outside the light's frustum

  // PCF for soft shadows
  // Sampling 9 neighboring texels in the shadow map

  float shadow = 0.0f;
  vec2 texelSize = 1.0 / textureSize(uDirectionalShadowMaps, 0).xy;
  for (int x = -1; x <= 1; ++x)
  {
    for (int y = -1; y <= 1; ++y)
    {
      float pcfClosestDepth = texture(
        uDirectionalShadowMaps, 
        vec3(projCoords.xy + vec2(x, y) * texelSize, shadowData.shadowMapIndex)
      ).r;

      float currentDepth = projCoords.z;
      shadow += currentDepth - bias > pcfClosestDepth ? 1.0 : 0.0;
    }
  }

  // Average the shadow factor over the 9 samples
  shadow /= 9.0;
  return 1.0 - shadow * shadowData.shadowStrength;
}

float calculateSpotLightShadow(
  int shadowFrameDataIndex,
  vec3 N, vec3 LDir, vec3 fragWorldPos
)
{
  if (shadowFrameDataIndex < 0 || shadowFrameDataIndex >= MAX_SPOT_LIGHTS_SHADOW_DATA)
  {
    return 1.0; // No shadow if the index is invalid
  }

  SpotLightShadowFrameData shadowData = spotLightShadowData[shadowFrameDataIndex];
  mat4 lightSpaceMatrix = shadowData.lightViewProjectionMatrix;
  float bias = max(shadowData.shadowBias * (1.0 - dot(N, LDir)), 0.002);

  vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragWorldPos, 1.0);
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0)
    return 1.0; // No shadow if fragment is outside the light's frustum

  if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
    return 1.0; // No shadow if fragment is outside the light's frustum

  // PCF for soft shadows
  // Sampling 9 neighboring texels in the shadow map

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(uSpotShadowMaps, 0).xy;
  for (int x = -1; x <= 1; ++x)
  {
    for (int y = -1; y <= 1; ++y)
    {
        float closestDepth = texture(
          uSpotShadowMaps, 
          vec3(projCoords.xy + vec2(x, y) * texelSize, shadowData.shadowMapIndex)
        ).r;
        float currentDepth = projCoords.z;

        closestDepth = linearizeDepth(closestDepth, shadowData.projectionNearPlane, shadowData.projectionFarPlane);
        currentDepth = linearizeDepth(currentDepth, shadowData.projectionNearPlane, shadowData.projectionFarPlane);

        shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }
  }

  // Average the shadow factor over the 9 samples
  shadow /= 9.0;
  return 1.0 - shadow * shadowData.shadowStrength;
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
    float shadowFactor = calculateDirectionalShadow(light.shadowFrameDataIndex, N, LDir, vWorldPos);

    specular += getSpecular(
      light.color,
      primaryColor, primaryShift,
      secondaryColor, secondaryShift,
      N, T, VDir, LDir,
      specularStrength,
      shininess,
      specularWidth
      /*, uv */ // uncomment when using specular shift texture
    ) * specularColor * light.color * lightIntensity * shadowFactor;
  }

  for (int i = 0; i < numOmniLights; ++i)
  {
    OmniLightData light = omniLights[i];
    vec3 LDir = normalize(light.position.xyz - vWorldPos);

    // Ominlight attenuation based on distance and range
    float distance = length(light.position.xyz - vWorldPos);
    float attenuation = calculateAttenuation(distance, light.range);
    float lightIntensity = light.intensity;

    specular += getSpecular(
      light.color,
      primaryColor, primaryShift,
      secondaryColor, secondaryShift,
      N, T, VDir, LDir,
      specularStrength,
      shininess,
      specularWidth
      /*, uv */ // uncomment when using specular shift texture
    ) * attenuation * specularColor * light.color * lightIntensity;
  }

  for (int i = 0; i < numSpotLights; ++i)
  {
    SpotLightData light = spotLights[i];
    vec3 LDir = normalize(light.position.xyz - vWorldPos);

    // Spot light attenuation based on distance and range
    float distance = length(light.position.xyz - vWorldPos);
    float attenuation = calculateAttenuation(distance, light.range);
    float lightIntensity = light.intensity;
    float shadowFactor = calculateSpotLightShadow(light.shadowFrameDataIndex, N, LDir, vWorldPos);

    specular += getSpecular(
      light.color,
      primaryColor, primaryShift,
      secondaryColor, secondaryShift,
      N, T, VDir, LDir,
      specularStrength,
      shininess,
      specularWidth
      /*, uv */ // uncomment when using specular shift texture
    ) * attenuation * specularColor * light.color * lightIntensity * shadowFactor;
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
  vec3 specularColor = specularSample.rgb;
  float shininess = uShininess * specularSample.a;

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