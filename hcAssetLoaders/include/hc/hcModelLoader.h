#pragma once

#include "hc/hcAssetLoadersPrerequisites.h"

struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace hc
{
  class HC_ASSET_LOADERS_EXPORT ModelLoader : public IAssetLoader<Model>
  {
  public:
    ModelLoader() = default;
    virtual ~ModelLoader() = default;

    /**
     * @copydoc IAssetLoader::load
     */
    SharedPtr<Model> load(const Path& filePath) override;

  private:
    UInt32 calculateTotalVertices(const aiScene* scene);
    UInt32 calculateTotalIndices(const aiScene* scene);
    void copyAssimpVertex(const aiMesh* mesh, UInt32 vertexIndex, Vertex& outVertex);    
  };
}
