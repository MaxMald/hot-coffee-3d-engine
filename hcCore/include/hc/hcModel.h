#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcAsset.h"
#include "hc/hcVertex.h"
#include "hc/hcModelSubMesh.h"

namespace hc
{
  class MaterialDescriptor;

  class HC_CORE_EXPORT Model : public Asset
  {
    Model();
    Model(
      const Buffer<Vertex>& vertices,
      const BufferUInt32& indices,
      const Vector<ModelSubMesh>& subMeshes,
      const Vector<SharedPtr<MaterialDescriptor>>& materials
    );
    ~Model() override;

    const Buffer<Vertex>& getVertices() const;
    const BufferUInt32& getIndices() const;
    const Vector<ModelSubMesh>& getSubMeshes() const;
    const Vector<SharedPtr<MaterialDescriptor>>& getMaterials() const;

  private:
    Buffer<Vertex> m_vertices;
    BufferUInt32 m_indices;
    Vector<ModelSubMesh> m_subMeshes;
    Vector<SharedPtr<MaterialDescriptor>> m_materials;
  };
}
