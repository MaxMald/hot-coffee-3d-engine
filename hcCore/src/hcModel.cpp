#include "hc/hcModel.h"

namespace hc
{
  Model::Model()
    : Asset(),
      m_vertices(0),
      m_indices(0)
  {
  }

  Model::~Model()
  {
  }

  const Buffer<Vertex>& Model::getVertices() const
  {
    return m_vertices;
  }

  const BufferUInt32& Model::getIndices() const
  {
    return m_indices;
  }
}
