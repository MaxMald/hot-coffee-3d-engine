#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcVector3.h"
#include "hc/hcVector2.h"
#include "hc/hcColor.h"

namespace hc
{
  struct HC_CORE_EXPORT Vertex
  {
    Vertex() = default;
    Vertex(
      const Vector3f& pos,
      const Vector3f& norm,
      const Vector2f& tex,
      const Color& col = Color(1.0f, 1.0f, 1.0f, 1.0f)
    );

    Vector3f position;
    Vector3f normal;
    Vector2f texCoord;
    Color color;
  };
}
