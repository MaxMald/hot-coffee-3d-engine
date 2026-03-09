#pragma once

#include "hc/hcAssetManagerPrerequisites.h"
#include "hc/assets/model/hcPrimitiveModelsFactory.h"

struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace hc
{
  /**
   * Concrete implementation of the model asset manager.
   *
   * Manages loading and caching of 3D model assets from files using
   * Assimp, as well as providing access to procedurally generated
   * primitive models. Loaded models are cached for reuse.
   */
  class ModelAssetManager : public IModelAssetManager
  {
  public:
    /**
     * Constructs a model asset manager.
     *
     * @param materialDescriptorAssetManager Reference to the material
     * descriptor manager for loading model materials
     */
    ModelAssetManager(
      IMaterialDescriptorAssetManager& materialDescriptorAssetManager
    );

    /** Virtual destructor */
    virtual ~ModelAssetManager() = default;

    /**
     * Loads a model from the specified path.
     *
     * If the model is already loaded, returns the cached instance.
     *
     * @param path The file path to the model asset
     * 
     * @return A shared pointer to the loaded model, or nullptr on failure
     */
    SharedPtr<Model> load(const Path& path) override;

    /**
     * Retrieves a previously loaded model.
     *
     * @param path The file path to the model asset
     * 
     * @return A shared pointer to the model if loaded, nullptr otherwise
     */
    SharedPtr<Model> get(const Path& path) const override;

    /**
     * Checks if a model at the specified path is currently loaded.
     *
     * @param path The file path to check
     * 
     * @return true if the model is loaded, false otherwise
     */
    bool isLoaded(const Path& path) const override;

    /**
     * Clears all loaded models from the cache.
     */
    void clear() override;

    /**
     * Retrieves all loaded models managed by this asset manager.
     *
     * @param outAssets A vector to be populated with shared pointers to all
     * loaded models. Vector will be cleared before being populated.
     */
    void getAllLoadedAssets(Vector<SharedPtr<Model>>& outAssets) const override;

    /**
     * Gets the number of models currently loaded in the manager.
     *
     * @return The number of loaded models
     */
    SizeT size() const override;

    /**
     * Retrieves a primitive shape model.
     *
     * Generates the primitive on first access and caches it for reuse.
     *
     * @param primitiveType The type of primitive shape to retrieve
     * 
     * @return A shared pointer to the primitive model
     */
    SharedPtr<Model> getPrimitive(primitiveModelType::Type primitiveType) override;

  private:
    UnorderedMap<Path, SharedPtr<Model>> m_loadedModels;
    UnorderedMap<primitiveModelType::Type, SharedPtr<Model>> m_primitiveModels;
    PrimitiveModelsFactory m_primitiveModelsFactory;

    /**
     * Calculates the total number of vertices across all meshes.
     *
     * @param scene The Assimp scene to analyze
     * 
     * @return The total vertex count
     */
    UInt32 calculateTotalVertices(const aiScene* scene);

    /**
     * Calculates the total number of indices across all meshes.
     *
     * @param scene The Assimp scene to analyze
     * 
     * @return The total index count
     */
    UInt32 calculateTotalIndices(const aiScene* scene);

    /**
     * Copies vertex data from an Assimp mesh to the engine's format.
     *
     * @param mesh The source Assimp mesh
     * 
     * @param vertexIndex The index of the vertex to copy
     * 
     * @param outVertex The destination vertex to populate
     */
    void copyAssimpVertex(const aiMesh* mesh, UInt32 vertexIndex, Vertex& outVertex);

    /**
     * Checks if a primitive type is already cached.
     *
     * @param primitiveType The primitive type to check
     * 
     * @return true if the primitive is cached, false otherwise
     */
    bool hasCachedPrimitive(primitiveModelType::Type primitiveType) const;
  };
}
