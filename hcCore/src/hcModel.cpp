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

  const Vector<ModelSubMesh>& Model::getSubMeshes() const
  {
    return m_subMeshes;
  }

  const Vector<SharedPtr<MaterialDescriptor>>& Model::getMaterials() const
  {
    return m_materials;
  }
}
