#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlMesh;
  class OpenGlGraphicsManager;
  class OpenGlMaterialManager;

  class OpenGlMeshManager : public IMeshManager
  {
  public:
    OpenGlMeshManager();
    ~OpenGlMeshManager() override;

    SharedPtr<IMesh> createMeshFromPath(const Path& path) override;
    SharedPtr<IMesh> createMeshFromModel(const SharedPtr<Model>& model) override;

  private:
    SharedPtr<AssetManager> m_assetManager;
    OpenGlMaterialManager* m_materialManager;
    UnorderedMap<Id, SharedPtr<OpenGlMesh>> m_cachedMeshes;

    void initialize(
      SharedPtr<AssetManager> assetManager,
      OpenGlMaterialManager* materialManager
    );

    friend class OpenGlGraphicsManager;
  };
}
