#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace builtInShaders
  {
    inline const char* vertexShaderSource = R"(
      #version 330 core
      layout(location = 0) in vec3 aPos;
      void main()
      {
          gl_Position = vec4(aPos, 1.0);
      }
    )";

    inline const char* fragmentShaderSource = R"(
      #version 330 core
      out vec4 FragColor;
      void main()
      {
          FragColor = vec4(1.0, 0.5, 0.2, 1.0);
      }
    )";
  }
}
