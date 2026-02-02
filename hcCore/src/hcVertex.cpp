#include "hc/hcVertex.h"

namespace hc
{
  Vertex::Vertex(
    const Vector3f& pos,
    const Vector3f& norm,
    const Vector3f& tang,
    const Vector2f& tex,
    const Color& col
  ) : position(pos),
    normal(norm),
    tangent(tang),
    texCoord(tex),
    color(col)
  {
  }
}
