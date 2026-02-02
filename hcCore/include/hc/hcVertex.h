#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  struct HC_CORE_EXPORT Vertex
  {
    Vertex() = default;
    Vertex(
      const Vector3f& pos,
      const Vector3f& norm,
      const Vector3f& tang,
      const Vector2f& tex,
      const Color& col = Color(1.0f, 1.0f, 1.0f, 1.0f)
    );

    Vector3f position;
    Vector3f normal;
    Vector3f tangent;
    Vector2f texCoord;
    Color color;
  };
}
