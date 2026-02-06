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

    const Vector<SharedPtr<IMaterial>>& getMaterials() const override;

    void clear() override;

  private:
    ITextureManager* m_textureManager;
    IShaderProgramManager* m_shaderProgramManager;
    SharedPtr<AssetManager> m_assetManager;
    Vector<SharedPtr<IMaterial>> m_materials;
    UnorderedMap<Id, SharedPtr<IMaterial>> m_cachedMaterialsByDescriptorId;
    SharedPtr<UnlitMaterial> m_defaultUnlitMaterial;

    void initialize(
      SharedPtr<AssetManager> assetManager,
      ITextureManager* textureManager,
      IShaderProgramManager* shaderProgramManager
    );

    SharedPtr<UnlitMaterial> createUnlitMaterial(
      SharedPtr<MaterialDescriptor> descriptor
    );

    bool hasCachedMaterialForDescriptorId(const Id& descriptorId) const;
    void addMaterial(SharedPtr<IMaterial> material);

    friend class OpenGlGraphicsManager;
  };
}
