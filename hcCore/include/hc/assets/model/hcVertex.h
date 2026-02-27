#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  struct HC_CORE_EXPORT Vertex
  {
    Vertex() :
      position(0.0f, 0.0f, 0.0f),
      normal(0.0f, 1.0f, 0.0f),
      tangent(1.0f, 0.0f, 0.0f),
      texCoord(0.0f, 0.0f),
      color(1.0f, 1.0f, 1.0f, 1.0f)
    {
    }

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
