#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlMesh;
  class OpenGlGraphicsManager;
  class OpenGlMaterialManager;

  /**
   * @brief Manages OpenGL mesh resources and their lifecycle.
   *
   * Responsible for creating, caching, and managing OpenGL mesh instances, as
   * well as providing access to all loaded meshes.
   */
  class OpenGlMeshManager : public IMeshManager
  {
  public:
    OpenGlMeshManager();
    ~OpenGlMeshManager() override;

    /**
     * @brief Creates a mesh from a file path.
     * 
     * @param path The file path to load the mesh from.
     * 
     * @return Shared pointer to the created mesh.
     */
    SharedPtr<IMesh> createMeshFromPath(const Path& path) override;

    /**
     * @brief Creates a mesh from a model instance.
     * 
     * @param model Shared pointer to the model to create the mesh from.
     * 
     * @return Shared pointer to the created mesh.
     */
    SharedPtr<IMesh> createMeshFromModel(const SharedPtr<Model>& model) override;

    /**
     * @brief Returns a constant reference to the list of all managed meshes.
     * 
     * @return Vector of shared pointers to all meshes.
     */
    const Vector<SharedPtr<IMesh>>& getMeshes() const override;

    /**
     * @brief Clears all managed meshes and cached mesh data.
     */
    void clear() override;

  private:
    Vector<SharedPtr<IMesh>> m_meshes;
    UnorderedMap<Id, SharedPtr<OpenGlMesh>> m_cachedMeshesByModelId;

    SharedPtr<AssetManager> m_assetManager;
    OpenGlMaterialManager* m_materialManager;

    /**
     * @brief Initializes the mesh manager with asset and material managers.
     * 
     * @param assetManager Shared pointer to the asset manager.
     * 
     * @param materialManager Pointer to the OpenGL material manager.
     */
    void initialize(
      SharedPtr<AssetManager> assetManager,
      OpenGlMaterialManager* materialManager
    );

    bool hasCachedMeshForModelId(const Id& modelId) const;
    void addMesh(const SharedPtr<OpenGlMesh>& mesh);

    friend class OpenGlGraphicsManager;
  };
}
