#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace builtInShaders
  {
    inline const String VertexShader = R"(
      #version 330 core
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
      #version 330 core
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
      #version 330 core
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
      #version 330 core
      in vec2 vTexCoord;
      in vec3 vNormal;
      in vec3 vTangent;
      in vec4 vColor;

      out vec4 FragColor;

      uniform vec4 uColor;
      uniform sampler2D uAlbedo;
      uniform sampler2D uNormalMap;
      uniform sampler2D uSpecularMap;
      uniform float uAlphaCutoff;
      uniform vec3 uCameraPosition;

      struct Light
      {
        vec3 position;
        vec3 color;
        float intensity;
      };

      uniform Light uLight;

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

        vec3 lightDir = normalize(uLight.position - vWorldPos);
        vec3 viewDir = normalize(uCameraPosition - vWorldPos);
        vec3 halfDir = normalize(lightDir + viewDir);

        float diff = max(dot(normalWS, lightDir), 0.0);
        float specBase = pow(max(dot(normalWS, halfDir), 0.0), 16.0);
        float specStrength = texture(uSpecularMap, vTexCoord).r;
        float spec = specBase * specStrength;

        vec3 lighting = (diff + spec) * uLight.color * uLight.intensity;
        FragColor = vec4(baseColor.rgb * lighting, baseColor.a);
      }
    )";
  }
}
