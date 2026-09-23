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
 * @brief Calculates the Trowbridge-Reitz GGX normal distribution function for a
 * given normal, half-vector, and roughness.
 *
 * @param N The surface normal vector.
 * @param H The half-vector between the view and light directions.
 * @param roughness The roughness of the surface.
 *
 * @return The GGX distribution value.
 */
float distributionGGX(vec3 N, vec3 H, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float denom = NdotH2 * (a2 - 1.0) + 1.0;
  denom = PI * denom * denom;
  return a2 / max(denom, EPSILON);
}

/**
 * @brief Calculates the Schlick-GGX geometry term for a given NdotX and
 * roughness.
 *
 * @param NdotX The dot product of the normal and the view or light direction.
 * @param roughness The roughness of the surface.
 *
 * @return The geometry term for the given NdotX and roughness.
 */
float geometrySchlickGGX(float NdotX, float roughness)
{
  const float r = roughness + 1.0;
  const float k = (r * r) / 8.0;
  return NdotX / max(NdotX * (1.0 - k) + k, EPSILON);
}

/**
 * @brief Calculates the Smith geometry term for a given normal, view, light,
 * and roughness.
 *
 * @param N The surface normal vector.
 * @param V The view direction vector.
 * @param L The light direction vector.
 * @param roughness The roughness of the surface.
 *
 * @return The Smith geometry term for the given inputs.
 */
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx1 = geometrySchlickGGX(NdotV, roughness);
  float ggx2 = geometrySchlickGGX(NdotL, roughness);
  return ggx1 * ggx2;
}

/**
 * @brief Calculates the Fresnel-Schlick approximation for a given cosine of the
 * angle and base reflectivity.
 *
 * @param cosTheta The cosine of the angle between the view direction and the
 * half-vector.
 * @param F0 The base reflectivity of the surface.
 *
 * @return The Fresnel term for the given inputs.
 */
vec4 fresnelSchlick(float cosTheta, vec4 F0)
{
  float fresnelFactor = pow(saturate(1.0 - cosTheta), 5.0);
  return F0 + (vec4(1.0) - F0) * fresnelFactor;
}

/**
 * @brief Evaluates the Physically Based Rendering (PBR) shading model for a
 * given set of inputs.
 *
 * This function implements the Cook-Torrance microfacet model for PBR shading.
 *
 * @param baseColor The base color of the surface.
 * @param radiance The incoming radiance from the light source.
 * @param N The surface normal vector.
 * @param V The view direction vector.
 * @param L The light direction vector.
 * @param F0 The base reflectivity of the surface.
 * @param roughness The roughness of the surface.
 * @param metallic The metallic property of the surface.
 *
 * @return The final color of the fragment after applying the PBR shading model.
 */
vec4 evaluatePBR(
  vec4 baseColor,
  vec4 radiance,
  vec3 N, 
  vec3 V, 
  vec3 L, 
  vec4 F0, 
  float roughness, 
  float metallic
)
{
  vec3 H = normalize(V + L);
  float NdotL = max(dot(N, L), 0.0);  // Light area
  float NdotV = max(dot(N, V), 0.0);  // Light area that I can see
  float NdotH = max(dot(N, H), 0.0);  // Specular area
  float VdotH = max(dot(V, H), 0.0);  // Specular area that I can see

  float D = distributionGGX(N, H, roughness);
  float G = geometrySmith(N, V, L, roughness);
  vec4 F = fresnelSchlick(VdotH, F0);

  vec4 numerator = D * G * F;
  float denominator = 4.0 * max(NdotV, 0.0) * max(NdotL, 0.0);
  vec4 specular = numerator / max(denominator, EPSILON);

  // (kD * albedo) / PI We need KS, but to calculate it we need to know the
  // Fresnel term F, which is already calculated above. To calculate the kD we
  // use the formula kD = 1 - F. In other words, it is what is not reflected,
  // but absorbed by the material.
  vec4 kS = F;
  vec4 kD = vec4(1.0) - kS;

  // Metals do not have a diffuse component, so we multiply kD by (1 - metallic)
  kD *= 1.0 - metallic; 

  vec4 diffuse = kD * baseColor;
  return (diffuse + specular) * radiance * NdotL;
}

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
  float attenuation = clamp(1.0 - distance / max(range, EPSILON), 0.001, 1.0);
  return attenuation * attenuation; // quadratic falloff
}

/**
* @brief Calculates the contribution of a directional light to the final color
* of a fragment.
*
* @param lightIndex The index of the directional light in the directionalLights array.
* @param baseColor The base diffuse color of the fragment.
* @param normal The normal vector at the fragment's surface.
* @param viewDir The direction from the fragment to the camera/viewer.
* @param worldPos The world position of the fragment.
* @param roughness The roughness of the fragment's material.
* @param metallic The metallic property of the fragment's material.
* @param F0 The base reflectivity of the fragment's material.
*
* @return The final color contribution of the directional light as a vec4.
*/
vec4 calculateDirectionalLightContribution(
  int lightIndex,
  vec4 baseColor,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  float roughness,
  float metallic,
  vec4 F0
)
{
  if (lightIndex < 0 || lightIndex >= MAX_DIRECTIONAL_LIGHTS)
    return vec4(0.0, 0.0, 0.0, 1.0);

  DirectionalLightData light = directionalLights[lightIndex];

  vec3 lightDir = normalize(-light.directionAndIntensity.xyz);
  vec4 radiance = light.color * light.directionAndIntensity.w;

  float shadowFactor = evaluateDirectionalShadowFactor(
    light.shadowFrameDataIndex,
    worldPos,
    normal,
    lightDir
  );

  radiance *= shadowFactor;

  vec4 pbrColor = evaluatePBR(
    baseColor,
    radiance,
    normal, viewDir, lightDir, F0,
    roughness,
    metallic
  );

  return pbrColor;
}

/**
* @brief Calculates the contribution of a spot light to the final color
* of a fragment.
*
* @param lightIndex The index of the spot light in the spotLights array.
* @param baseColor The base diffuse color of the fragment.
* @param normal The normal vector at the fragment's surface.
* @param viewDir The direction from the fragment to the camera/viewer.
* @param worldPos The world position of the fragment.
* @param roughness The roughness of the fragment's material.
* @param metallic The metallic property of the fragment's material.
* @param F0 The base reflectivity of the fragment's material.
*
* @return The final color contribution of the spot light as a vec4.
*/
vec4 calculateSpotLightContribution(
  int lightIndex,
  vec4 baseColor,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  float roughness,
  float metallic,
  vec4 F0
)
{
  if (lightIndex < 0 || lightIndex >= MAX_SPOT_LIGHTS)
    return vec4(0.0, 0.0, 0.0, 1.0);

  SpotLightData light = spotLights[lightIndex];
  vec3 lightDir = normalize(light.position.xyz - worldPos);
  
  float theta = dot(-light.direction.xyz, lightDir);
  if (theta <= light.outerConeCos)
    return vec4(0.0, 0.0, 0.0, 1.0);

  // Light attenuation and spotlight cone calculations

  float distance = length(light.position.xyz - worldPos);
  float attenuation = calculateAttenuation(distance, light.range);

  float epsilon = clamp(light.innerConeCos - light.outerConeCos, 0.001, 1.0);
  float spillLightIntensity = clamp((theta - light.outerConeCos) / epsilon, 0.0, 1.0);

  vec4 radiance = light.color * light.intensity * attenuation * spillLightIntensity;

  float shadowFactor = evaluateSpotLightShadowFactor(
    light.shadowFrameDataIndex, 
    worldPos, 
    normal, 
    lightDir
  );

  radiance *= shadowFactor;

  // PBR color calculation

  vec4 pbrColor = evaluatePBR(
    baseColor, 
    radiance, 
    normal, viewDir, lightDir, F0,
    roughness, metallic
  );

  return pbrColor;
}

/**
* @brief Calculates the contribution of an omni-directional light to the final color
* of a fragment.
*
* @param lightIndex The index of the omni light in the omniLights array.
* @param baseColor The base diffuse color of the fragment.
* @param normal The normal vector at the fragment's surface.
* @param viewDir The direction from the fragment to the camera/viewer.
* @param worldPos The world position of the fragment.
* @param roughness The roughness of the fragment's material.
* @param metallic The metallic property of the fragment's material.
* @param F0 The base reflectivity of the fragment's material.
*
* @return The final color contribution of the omni light as a vec4.
*/
vec4 calculateOmniLightContribution(
  int lightIndex, 
  vec4 baseColor,
  vec3 normal, 
  vec3 viewDir, 
  vec3 worldPos,
  float roughness,
  float metallic,
  vec4 F0
)
{
  if (lightIndex < 0 || lightIndex >= MAX_OMNI_LIGHTS)
    return vec4(0.0, 0.0, 0.0, 1.0);

  OmniLightData light = omniLights[lightIndex];
  vec3 lightDir = normalize(light.position.xyz - worldPos);

// Calculate attenuation based on distance and light range

  float distance = length(light.position.xyz - worldPos);
  float attenuation = calculateAttenuation(distance, light.range);

  vec4 radiance = light.color * light.intensity * attenuation;

  // Evaluate the PBR color for the omni light

  vec4 pbrColor = evaluatePBR(
    baseColor, 
    light.color, 
    normal, viewDir, lightDir, F0,
    roughness, metallic
  );

  return pbrColor;
}

/**
* @brief Evaluates the outgoing radiance from a fragment considering all lights
* in the scene.
*
* @param baseColor The base diffuse color of the fragment.
* @param normal The normal vector at the fragment's surface.
* @param viewDir The direction from the fragment to the camera/viewer.
* @param worldPos The world position of the fragment.
* @param roughness The roughness of the fragment's material.
* @param metallic The metallic property of the fragment's material.
* @param ior The index of refraction of the fragment's material.
*
* @return The outgoing radiance from the fragment as a vec4.
*/
vec4 evaluateOutgoingRadiance(
  vec4 baseColor,
  vec3 normal,
  vec3 viewDir,
  vec3 worldPos,
  float roughness,
  float metallic,
  float ior
)
{

  float F0 = pow((ior - 1.0) / (ior + 1.0), 2.0); // Fresnel reflectance at normal incidence
  vec4 F0Color = mix(vec4(F0), baseColor, metallic);

  vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);
  for (int i = 0; i < numOmniLights; ++i)
  {
    finalColor += calculateOmniLightContribution(
      i,
      baseColor,
      normal, 
      viewDir, 
      worldPos, 
      roughness,
      metallic,
      F0Color
    );
  }

  for (int i = 0; i < numDirectionalLights; ++i)
  {
    finalColor += calculateDirectionalLightContribution(
      i, 
      baseColor,
      normal, 
      viewDir, 
      worldPos, 
      roughness,
      metallic,
      F0Color
    );
  }

  for (int i = 0; i < numSpotLights; ++i)
  {
    finalColor += calculateSpotLightContribution(
      i, 
      baseColor,
      normal, 
      viewDir, 
      worldPos, 
      roughness,
      metallic,
      F0Color
    );
  }

  return finalColor;
}
