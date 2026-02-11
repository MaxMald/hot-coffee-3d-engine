#pragma once

#include <hc/hcATypedAssetLoader.h>
#include "hc/hcAssetLoadersPrerequisites.h"

struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace hc
{
  /**
   * @brief Asset loader for 3D model resources.
   */
  class HC_ASSET_LOADERS_EXPORT ModelLoader : public ATypedAssetLoader<Model>
  {
  public:
    ModelLoader() = default;
    virtual ~ModelLoader() = default;

    /**
     * @brief Loads a model asset from the specified file path.
     *
     * @param filePath The path to the model file to load.
     * @return Shared pointer to the loaded model asset, or nullptr on failure.
     */
    SharedPtr<Model> load(const Path& filePath) override;

  private:
    UInt32 calculateTotalVertices(const aiScene* scene);
    UInt32 calculateTotalIndices(const aiScene* scene);
    void copyAssimpVertex(const aiMesh* mesh, UInt32 vertexIndex, Vertex& outVertex);
  };
}
