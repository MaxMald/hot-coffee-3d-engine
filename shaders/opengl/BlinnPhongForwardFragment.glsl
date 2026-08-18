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

in vec2 vTexCoord;
in vec3 vWorldPos;
in vec3 vNormal;
in vec3 vTangent;
in vec4 vColor;

out vec4 FragColor;

uniform vec4 uColor;
uniform sampler2D uAlbedo;
uniform sampler2D uNormalMap;
uniform sampler2D uSpecularMap;
uniform float uAlphaCutoff;
uniform float uShininess;

float calculateAttenuation(float distance, float range)
{
  // Quadratic attenuation that clamps to 0 at the light's range
  float attenuation = clamp(1.0 - distance / range, 0.001, 1.0);
  return attenuation * attenuation; // quadratic falloff
}

vec3 calculateOmniLight(OmniLightData light, vec3 normal, vec3 viewDir, vec3 worldPos, float shininess)
{
  vec3 lightDir = normalize(light.position.xyz - worldPos);
  vec3 halfDir = normalize(lightDir + viewDir);

  float diff = max(dot(normal, lightDir), 0.0);
  float specBase = pow(max(dot(normal, halfDir), 0.0), shininess);
  float specStrength = texture(uSpecularMap, vTexCoord).r;
  float spec = specBase * specStrength;
  float distance = length(light.position.xyz - worldPos);
  float attenuation = calculateAttenuation(distance, light.range);

  return (diff + spec) * light.color.rgb * light.intensity * attenuation;
}

vec3 calculateDirectionalLight(DirectionalLightData light, vec3 normal, vec3 viewDir, float shininess)
{
  vec3 lightDir = normalize(-light.directionAndIntensity.xyz);
  vec3 halfDir = normalize(lightDir + viewDir);

  float diff = max(dot(normal, lightDir), 0.0);
  float specBase = pow(max(dot(normal, halfDir), 0.0), shininess);
  float specStrength = texture(uSpecularMap, vTexCoord).r;
  float spec = specBase * specStrength;

  return (diff + spec) * light.color.rgb * light.directionAndIntensity.w;
}

vec3 calculateSpotLight(SpotLightData light, vec3 normal, vec3 viewDir, vec3 worldPos, float shininess)
{
  vec3 lightDir = normalize(light.position.xyz - worldPos);
  vec3 halfDir = normalize(lightDir + viewDir);

  float diff = max(dot(normal, lightDir), 0.0);
  float specBase = pow(max(dot(normal, halfDir), 0.0), shininess);
  float specStrength = texture(uSpecularMap, vTexCoord).r;
  float spec = specBase * specStrength;
  float distance = length(light.position.xyz - worldPos);
  float attenuation = calculateAttenuation(distance, light.range);

  float theta = dot(-light.direction.xyz, lightDir);
  if (theta > light.outerConeCos)
  {
    float epsilon = clamp(light.innerConeCos - light.outerConeCos, 0.001, 1.0);
    float intensity = clamp((theta - light.outerConeCos) / epsilon, 0.0, 1.0);
    return (diff + spec) * light.color.rgb * light.intensity * attenuation * intensity;
  }
  else
  {
    return vec3(0.0);
  }
}

void main()
{
  vec4 albedoTex = texture(uAlbedo, vTexCoord);
  if (uAlphaCutoff > 0.0 && albedoTex.a < uAlphaCutoff)
    discard;

  vec4 baseColor = uColor * vColor * albedoTex;

  vec3 N = normalize(vNormal);
  vec3 T = normalize(vTangent - dot(vTangent, N) * N);
  vec3 B = normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  vec3 normalTS = texture(uNormalMap, vTexCoord).xyz * 2.0 - 1.0;
  vec3 normalWS = normalize(TBN * normalTS);

  vec3 viewDir = normalize(cameraPosition - vWorldPos);
  vec3 totalLighting = vec3(0.05);

  for (int i = 0; i < numOmniLights; ++i)
    totalLighting += calculateOmniLight(omniLights[i], normalWS, viewDir, vWorldPos, uShininess);

  for (int i = 0; i < numDirectionalLights; ++i)
    totalLighting += calculateDirectionalLight(directionalLights[i], normalWS, viewDir, uShininess);

  for (int i = 0; i < numSpotLights; ++i)
    totalLighting += calculateSpotLight(spotLights[i], normalWS, viewDir, vWorldPos, uShininess);

  FragColor = vec4(baseColor.rgb * totalLighting, baseColor.a);
}