#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace builtInShaders
  {
    inline const String UnlitVertex = R"(
      #version 420 core

      layout(std140, binding = 1) uniform CameraFrameBlock
      {
        mat4 projection;
        mat4 view;
        vec3 cameraPosition;
        float cPadding0;
      };

      layout(location = 0) in vec3 aPosition;
      layout(location = 1) in vec3 aNormal;
      layout(location = 2) in vec3 aTangent;
      layout(location = 3) in vec2 aTexCoord;
      layout(location = 4) in vec4 aColor;

      uniform mat4 uModel;

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

        gl_Position = projection * view * uModel * vec4(aPosition, 1.0);
      }
    )";

    inline const String LitVertex = R"(
      #version 420 core

      layout(std140, binding = 1) uniform CameraFrameBlock
      {
        mat4 projection;
        mat4 view;
        vec3 cameraPosition;
        float cPadding0;
      };

      layout(location = 0) in vec3 aPosition;
      layout(location = 1) in vec3 aNormal;
      layout(location = 2) in vec3 aTangent;
      layout(location = 3) in vec2 aTexCoord;
      layout(location = 4) in vec4 aColor;

      uniform mat4 uModel;

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

        gl_Position = projection * view * worldPos;
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
    )";

    inline const String BlinnPhongDeferredFragment = R"(
      #version 420 core

      in vec2 vTexCoord;
      in vec3 vWorldPos;
      in vec3 vNormal;
      in vec3 vTangent;
      in vec4 vColor;

      layout(location = 0) out vec4 OutPositionAndDepth;
      layout(location = 1) out vec4 OutNormalRoughness;
      layout(location = 2) out vec4 OutAlbedoAlpha;
      layout(location = 3) out vec4 OutMaterialParameters;

      uniform vec4 uColor;
      uniform sampler2D uAlbedo;
      uniform sampler2D uNormalMap;
      uniform sampler2D uSpecularMap;
      uniform float uAlphaCutoff;
      uniform float uShininess;

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

        OutPositionAndDepth = vec4(vWorldPos, gl_FragCoord.z);
        OutNormalRoughness = vec4(normalWS, 1.0 - (uShininess / 256.0));
        OutAlbedoAlpha = baseColor;
        OutMaterialParameters = vec4(texture(uSpecularMap, vTexCoord).r, 0.0, 0.0, 1.0);
      }
    )";

    inline const String FullScreenTriangleVertex = R"(
      #version 420 core

      out vec2 vTexCoord;

      void main()
      {
        // Generate a full-screen triangle using vertex ID
        // 3 vertices cover the entire screen
        vec2 positions[3] = vec2[](
          vec2(-1.0, -1.0),
          vec2( 3.0, -1.0),
          vec2(-1.0,  3.0)
        );

        vec2 texCoords[3] = vec2[](
          vec2(0.0, 0.0),
          vec2(2.0, 0.0),
          vec2(0.0, 2.0)
        );

        vTexCoord = texCoords[gl_VertexID];
        gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
      }
    )";

    inline const String DeferredLightingFragment = R"(
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
        int lPadding0;
      };

      layout(binding = 0) uniform sampler2D uPositionAndDepth;
      layout(binding = 1) uniform sampler2D uNormalRoughness;
      layout(binding = 2) uniform sampler2D uAlbedoAlpha;
      layout(binding = 3) uniform sampler2D uMaterialParameters;

      in vec2 vTexCoord;

      out vec4 FragColor;

      float calculateAttenuation(float distance, float range)
      {
        // Quadratic attenuation that clamps to 0 at the light's range
        float attenuation = clamp(1.0 - distance / range, 0.001, 1.0);
        return attenuation * attenuation; // quadratic falloff
      }

      vec3 calculateOmniLight(OmniLightData light, vec3 normal, vec3 viewDir, vec3 worldPos, float specularStrength, float shininess)
      {
        vec3 lightDir = normalize(light.position.xyz - worldPos);
        vec3 halfDir = normalize(lightDir + viewDir);

        float diff = max(dot(normal, lightDir), 0.0);
        float specBase = pow(max(dot(normal, halfDir), 0.0), shininess);
        float spec = specBase * specularStrength;
        float distance = length(light.position.xyz - worldPos);
        float attenuation = calculateAttenuation(distance, light.range);

        return (diff + spec) * light.color.rgb * light.intensity * attenuation;
      }

      vec3 calculateDirectionalLight(DirectionalLightData light, vec3 normal, vec3 viewDir, float specularStrength, float shininess)
      {
        vec3 lightDir = normalize(-light.directionAndIntensity.xyz);
        vec3 halfDir = normalize(lightDir + viewDir);

        float diff = max(dot(normal, lightDir), 0.0);
        float specBase = pow(max(dot(normal, halfDir), 0.0), shininess);
        float spec = specBase * specularStrength;

        return (diff + spec) * light.color.rgb * light.directionAndIntensity.w;
      }

      vec3 calculateSpotLight(SpotLightData light, vec3 normal, vec3 viewDir, vec3 worldPos, float specularStrength, float shininess)
      {
        vec3 lightDir = normalize(light.position.xyz - worldPos);
        vec3 halfDir = normalize(lightDir + viewDir);

        float diff = max(dot(normal, lightDir), 0.0);
        float specBase = pow(max(dot(normal, halfDir), 0.0), shininess);
        float spec = specBase * specularStrength;
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

        vec3 totalLighting = vec3(0.05);
        for (int i = 0; i < numOmniLights; ++i)
          totalLighting += calculateOmniLight(omniLights[i], normal, viewDir, worldPos, specularStrength, shininess);
        for (int i = 0; i < numDirectionalLights; ++i)
          totalLighting += calculateDirectionalLight(directionalLights[i], normal, viewDir, specularStrength, shininess);
        for (int i = 0; i < numSpotLights; ++i)
          totalLighting += calculateSpotLight(spotLights[i], normal, viewDir, worldPos, specularStrength, shininess);

        FragColor = vec4(albedo * totalLighting, alpha);
      }
    )";

    inline const String SkyboxVertex = R"(
      #version 420 core

      layout(std140, binding = 1) uniform CameraFrameBlock
      {
        mat4 projection;
        mat4 view;
        vec3 cameraPosition;
        float cPadding0;
      };

      layout(location = 0) in vec3 aPosition;

      out vec3 vDirection;

      void main()
      {
        vDirection = vec3(aPosition.xy, -aPosition.z); // Flip Z to match OpenGL's coordinate system
        gl_Position = projection * mat4(mat3(view)) * vec4(aPosition, 1.0);
        gl_Position = gl_Position.xyww; // Force depth to 1.0 to ensure skybox is rendered behind all geometry
      }
    )";

    inline const String SkyboxFragment = R"(
      #version 420 core

      in vec3 vDirection;

      out vec4 FragColor;

      layout(binding = 0) uniform samplerCube uSkybox;

      void main()
      {
        FragColor = texture(uSkybox, vDirection);
      }
    )";

    inline const String FinalPassFragment = R"(
      #version 420 core

      layout(binding = 0) uniform sampler2D uScene;

      in vec2 vTexCoord;
      out vec4 FragColor;

      void main()
      {
        vec4 color = texture(uScene, vTexCoord);
        color.rgb = pow(color.rgb, vec3(0.454545)); // Apply gamma correction (gamma = 2.2)
        FragColor = color;
      }
    )";
  }
}
