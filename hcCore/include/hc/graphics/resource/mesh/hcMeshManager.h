#pragma once

#include "hc/graphics/resource/mesh/hcIMeshManager.h"
#include "hc/graphics/resource/hcResourcesCache.h"

namespace hc
{
  class IMaterial;
  class IMaterialManager;
  class IMeshFactory;
  class IAssetManager;

  /**
   * @brief Manages mesh resources and their creation.
   *
   * Provides functionality for creating meshes from paths and models, caching
   * mesh instances, and managing associated materials. Inherits from
   * IMeshManager and uses ResourcesCache for mesh caching.
   */
  class HC_CORE_EXPORT MeshManager :
    public IMeshManager,
    private ResourcesCache<Id, IMesh>
  {
  public:
    /**
     * @brief Constructs a MeshManager with required dependencies.
     *
     * @param assetManager Reference to the asset manager.
     * @param meshFactory Unique pointer to the mesh factory.
     * @param materialManager Reference to the material manager.
     */
    MeshManager(
      IAssetManager& assetManager,
      UniquePtr<IMeshFactory> meshFactory,
      IMaterialManager& materialManager
    );
    virtual ~MeshManager() = default;

    /**
     * @brief Creates a mesh from the specified file path.
     *
     * @param path Path to the mesh asset.
     *
     * @return Shared pointer to the created mesh instance. nullptr if loading
     * fails.
     */
    SharedPtr<IMesh> createMeshFromPath(const Path& path) override;

    /**
     * @brief Creates a mesh from the given model.
     *
     * @param model Shared pointer to the model used for mesh creation.
     *
     * @return Shared pointer to the created mesh instance. nullptr if creation
     * fails.
     */
    SharedPtr<IMesh> createMeshFromModel(const SharedPtr<Model>& model) override;

    /**
     * @brief Returns a vector of all managed mesh instances.
     *
     * @return Const reference to the vector of mesh instances.
     */
    const Vector<SharedPtr<IMesh>>& getMeshes() const override;

    /**
     * @brief Clears all managed meshes and cached resources.
     */
    void clear() override;

  private:
    IAssetManager& m_assetManager;
    UniquePtr<IMeshFactory> m_meshFactory;
    IMaterialManager& m_materialManager;
    Vector<SharedPtr<IMesh>> m_meshes;

    Vector<SharedPtr<IMaterial>> createMaterialsFromModel(
      const SharedPtr<Model>& model
    );
  };
}
