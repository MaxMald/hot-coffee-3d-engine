#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGraphicsManager;

  class OpenGlMaterialManager : public IMaterialManager
  {
  public:
    OpenGlMaterialManager();
    virtual ~OpenGlMaterialManager() override;

    SharedPtr<IMaterial> createMaterialFromFile(
      const Path& materialDescriptorPath
    ) override;

    SharedPtr<IMaterial> createMaterialFromDescriptor(
      const String& materialDescriptorKey
    ) override;

    SharedPtr<IMaterial> createMaterialFromDescriptor(
      SharedPtr<MaterialDescriptor> descriptor
    ) override;

    void clear() override;

  private:
    ITextureManager* m_textureManager;
    SharedPtr<AssetManager> m_assetManager;
    UnorderedMap<Id, SharedPtr<IMaterial>> m_cachedMaterials;

    void initialize(
      SharedPtr<AssetManager> assetManager,
      ITextureManager* textureManager
    );

    SharedPtr<UnlitMaterial> createUnlitMaterial(
      SharedPtr<MaterialDescriptor> descriptor
    );

    friend class OpenGlGraphicsManager;
  };
}
