#include "hc/assets/model/hcModel.h"

namespace hc
{
  Model::Model(
    const Path& path,
    const Buffer<Vertex>& vertices,
    const BufferUInt32& indices,
    const Vector<ModelSubMesh>& subMeshes,
    const Vector<SharedPtr<AMaterialDescriptor>>& materials
  ) :
    Asset(path),
    m_vertices(vertices),
    m_indices(indices),
    m_subMeshes(subMeshes),
    m_materials(materials)
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

  const Vector<SharedPtr<AMaterialDescriptor>>& Model::getMaterials() const
  {
    return m_materials;
  }
}
