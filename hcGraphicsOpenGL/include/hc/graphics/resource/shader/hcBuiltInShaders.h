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

    inline const String UnlitFragment = R"(
      #version 330 core
      in vec2 vTexCoord;
      in vec4 vColor;
      out vec4 FragColor;

      uniform vec4 uColor;
      uniform sampler2D uTexture;
      uniform bool uUseTexture;
      uniform float uAlphaCutoff;

      void main()
      {
        vec4 baseColor = uColor * vColor;
        if (uUseTexture)
        {
          vec4 textureColor = texture(uTexture, vTexCoord);
          if (uAlphaCutoff > 0.0 && textureColor.a < uAlphaCutoff)
            discard;

          baseColor *= textureColor;
        }
          
        FragColor = baseColor;
      }
    )";
  }
}
