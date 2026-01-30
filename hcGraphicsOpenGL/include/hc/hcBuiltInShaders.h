#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace builtInShaders
  {
    inline const String UnlitVertex = R"(
      #version 330 core
      layout(location = 0) in vec3 aPosition;
      layout(location = 1) in vec2 aTexCoord;

      uniform mat4 uModel;
      uniform mat4 uView;
      uniform mat4 uProjection;

      out vec2 vTexCoord;

      void main()
      {
        vTexCoord = aTexCoord;
        gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
      }
    )";

    inline const String UnlitFragment = R"(
      #version 330 core
      in vec2 vTexCoord;
      out vec4 FragColor;

      uniform vec4 uColor;
      uniform sampler2D uTexture;
      uniform bool uUseTexture;

      void main()
      {
        vec4 baseColor = uColor;
        if (uUseTexture)
          baseColor *= texture(uTexture, vTexCoord);
        FragColor = baseColor;
      }
    )";
  }
}
