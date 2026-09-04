#pragma once

#include "hc/graphics/hcGraphicsCommons.h"
#include "hc/graphics/resource/hcIGraphicResource.h"
#include "hc/graphics/hcIDrawable.h"

namespace hc
{
  class IMaterial;
  class Model;
  struct Vertex;
  struct ModelSubMesh;

  /**
   * @brief Interface for mesh objects in the engine.
   */
  class IMesh : public IGraphicResource, public IDrawable
  {
  public:
    virtual ~IMesh() = default;

    /**
     * @brief Initializes the mesh with the given model and materials.
     *
     * @param model Reference to the model to associate with this mesh.
     * @param materials A vector of shared pointers to the materials to use for this mesh.
     */
    virtual void initialize(
      const Model& model,
      const Vector<SharedPtr<IMaterial>>& materials
    ) = 0;

    /**
     * @brief Initializes the mesh with the given vertex and index buffers, and materials.
     *
     * @param vertices The vertex buffer to use for this mesh.
     * @param indices The index buffer to use for this mesh.
     * @param materials A vector of shared pointers to the materials to use for this mesh.
     */
    virtual void initialize(
      const Buffer<Vertex>& vertices,
      const BufferUInt32& indices,
      const Vector<SharedPtr<IMaterial>>& materials
    ) = 0;

    /**
     * @brief Initializes the mesh with the given vertex and index buffers, submeshes, and
     * materials.
     *
     * @param vertices The vertex buffer to use for this mesh.
     * @param indices The index buffer to use for this mesh.
     * @param subMeshes The collection of submeshes to define the mesh's geometry.
     * @param materials A vector of shared pointers to the materials to use for this mesh.
     */
    virtual void initialize(
      const Buffer<Vertex>& vertices,
      const BufferUInt32& indices,
      const Vector<ModelSubMesh>& subMeshes,
      const Vector<SharedPtr<IMaterial>>& materials
    ) = 0;

    /**
     * @brief Updates the mesh data or state based on the associated model.

     * This involves uploading vertex and index data to the GPU, updating submesh
     * information and source path.
     */
    virtual void update(const Model& model) = 0;

    /**
     * @brief Updates the mesh's vertex and index buffers with new data.
     *
     * This method allows for updating the geometry of the mesh without changing the
     * associated submeshes or materials.
     *
     * @param vertices The new vertex buffer to upload to the GPU.
     * @param indices The new index buffer to upload to the GPU.
     */
    virtual void update(const Buffer<Vertex>& vertices, const BufferUInt32& indices) = 0;

    /**
     * @brief Sets the material at the specified index for this mesh.
     *
     * @param index The index of the material to set.
     * @param material Shared pointer to the material to associate with this mesh.
     */
    virtual void setMaterial(UInt32 index, SharedPtr<IMaterial> material) = 0;

    /**
     * @brief Returns the number of materials associated with this mesh.
     */
    virtual SizeT getMaterialsSize() const = 0;

    /**
     * @brief Returns the material at the specified index.
     *
     * @param index The index of the material to retrieve.
     * 
     * @return Shared pointer to the material at the specified index.
     */
    virtual SharedPtr<IMaterial> getMaterial(UInt32 index) const = 0;

    /**
     * @brief Returns the materials associated with this mesh.
     * 
     * @return A vector of shared pointers to the materials.
     */
    virtual const Vector<SharedPtr<IMaterial>> getMaterials() = 0;

    /**
     * @brief Returns the draw type of this mesh.
     *
     * @return The draw type used for rendering this mesh.
     */
    virtual drawType::Type getDrawType() const = 0;

    /**
     * @brief Sets the draw type for this mesh.
     *
     * @param drawType The draw type to use for rendering this mesh.
     */
    virtual void setDrawType(drawType::Type drawType) = 0;

    /**
     * @brief Validates the mesh's state and configuration.
     *
     * @return True if the mesh is in a valid state and can be rendered, false otherwise.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief Returns the file path of the source model asset used to create this mesh.
     *
     * @note This path might be empty if the mesh was created procedurally or does not
     * have an associated source model file.
     *
     * @return The file path of the source model asset if available, otherwise an empty
     * path.
     */
    virtual Path getSourcePath() const = 0;

    /**
     * @brief Sets the file path of the source model asset used to create this mesh.
     *
     * This method allows updating the source path, which can be useful for tracking
     * the origin of the mesh data or for debugging purposes.
     *
     * @param path The file path to set as the source of this mesh.
     */
    virtual void setSourcePath(const Path& path) = 0;

  protected:
    IMesh() = default;
  };
}
