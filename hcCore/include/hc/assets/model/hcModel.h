#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/assets/hcAsset.h"
#include "hc/assets/model/hcVertex.h"
#include "hc/assets/model/hcModelSubMesh.h"

namespace hc
{
  class AMaterialDescriptor;

  /**
   * Represents a 3D model asset.
   *
   * Contains geometry data including vertices, indices, and submeshes,
   * along with associated material descriptors.
   */
  class HC_CORE_EXPORT Model : public Asset
  {
  public:
    /**
     * Constructs a model with the specified geometry and materials.
     *
     * @param path The file path to the model asset
     * @param vertices The vertex buffer containing position, normal, etc.
     * @param indices The index buffer for triangle definitions
     * @param subMeshes The collection of submeshes within the model
     * @param materials The material descriptors for each submesh
     */
    Model(
      const Path& path,
      const Buffer<Vertex>& vertices,
      const BufferUInt32& indices,
      const Vector<ModelSubMesh>& subMeshes,
      const Vector<SharedPtr<AMaterialDescriptor>>& materials
    );

    /** Virtual destructor */
    virtual ~Model() = default;

    /**
     * Gets the vertex buffer of the model.
     *
     * @return A constant reference to the vertex buffer
     */
    const Buffer<Vertex>& getVertices() const;

    /**
     * Gets the index buffer of the model.
     *
     * @return A constant reference to the index buffer
     */
    const BufferUInt32& getIndices() const;

    /**
     * Gets the submeshes contained in the model.
     *
     * @return A constant reference to the submesh collection
     */
    const Vector<ModelSubMesh>& getSubMeshes() const;

    /**
     * Gets the material descriptors associated with the model.
     *
     * @return A constant reference to the material descriptor collection
     */
    const Vector<SharedPtr<AMaterialDescriptor>>& getMaterials() const;

  private:
    Buffer<Vertex> m_vertices;
    BufferUInt32 m_indices;
    Vector<ModelSubMesh> m_subMeshes;
    Vector<SharedPtr<AMaterialDescriptor>> m_materials;
  };
}
