#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace builtInShaders
  {
    inline const String VertexShader = R"(
      #version 420 core
      layout(location = 0) in vec3 aPosition;
      layout(location = 1) in vec3 aNormal;
      layout(location = 2) in vec3 aTangent;
      layout(location = 3) in vec2 aTexCoord;
      layout(location = 4) in vec4 aColor;

      uniform mat4 uModel;
      uniform mat4 uView;
      uniform mat4 uProjection;

      out vec2 vTexCoord;
      out vec3 vNormal;
      out vec3 vTangent;
      out vec4 vColor;

      void main()
      {
        vTexCoord = aTexCoord;
        vNormal = mat3(uModel) * aNormal;
        vTangent = mat3(uModel) * aTangent;
        vColor = aColor;

        gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
      }
    )";

    inline const String LitVertexShader = R"(
      #version 420 core
      layout(location = 0) in vec3 aPosition;
      layout(location = 1) in vec3 aNormal;
      layout(location = 2) in vec3 aTangent;
      layout(location = 3) in vec2 aTexCoord;
      layout(location = 4) in vec4 aColor;

      uniform mat4 uModel;
      uniform mat4 uView;
      uniform mat4 uProjection;

      out vec2 vTexCoord;
      out vec3 vWorldPos;
      out vec3 vNormal;
      out vec3 vTangent;
      out vec4 vColor;

      void main()
      {
        vec4 worldPos = uModel * vec4(aPosition, 1.0);
        vWorldPos = worldPos.xyz;

        vTexCoord = aTexCoord;
        vNormal = normalize(mat3(uModel) * aNormal);
        vTangent = normalize(mat3(uModel) * aTangent);
        vColor = aColor;

        gl_Position = uProjection * uView * worldPos;
      }
    )";

    inline const String UnlitFragment = R"(
      #version 420 core
      in vec2 vTexCoord;
      in vec4 vColor;
      out vec4 FragColor;

      uniform vec4 uColor;
      uniform sampler2D uTexture;
      uniform float uAlphaCutoff;

      void main()
      {
        vec4 baseColor = uColor * vColor;
        vec4 textureColor = texture(uTexture, vTexCoord);

        if (uAlphaCutoff > 0.0 && textureColor.a < uAlphaCutoff)
          discard;

        FragColor = baseColor * textureColor;
      }
    )";

    inline const String BlinnPhongForwardFragment = R"(
      #version 420 core

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
      };

      layout(std140, binding = 2) uniform LightBlock
      {
        DirectionalLightData directionalLights[MAX_DIRECTIONAL_LIGHTS];
        OmniLightData omniLights[MAX_OMNI_LIGHTS];
        SpotLightData spotLights[MAX_SPOT_LIGHTS];
        int numDirectionalLights;
        int numOmniLights;
        int numSpotLights;
        int padding; ///< Padding to ensure 16-byte alignment.
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
      uniform vec3 uCameraPosition;

      float calculateAttenuation(float distance, float range)
      {
        // Quadratic attenuation that clamps to 0 at the light's range
        float attenuation = clamp(1.0 - distance / range, 0.0, 1.0);
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
          float epsilon = light.innerConeCos - light.outerConeCos;
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

        vec3 viewDir = normalize(uCameraPosition - vWorldPos);
        vec3 totalLighting = vec3(0.05);

        for (int i = 0; i < numOmniLights; ++i)
          totalLighting += calculateOmniLight(omniLights[i], normalWS, viewDir, vWorldPos, uShininess);

        for (int i = 0; i < numDirectionalLights; ++i)
          totalLighting += calculateDirectionalLight(directionalLights[i], normalWS, viewDir, uShininess);

        for (int i = 0; i < numSpotLights; ++i)
          totalLighting += calculateSpotLight(spotLights[i], normalWS, viewDir, vWorldPos, uShininess);

        FragColor = vec4(baseColor.rgb * totalLighting, baseColor.a);
      }
    )";
  }
}
