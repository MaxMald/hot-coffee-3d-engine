#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIDependencyResolvable.h"
#include "hc/hcIMaterial.h"

namespace hc
{
  class AssetManager;
  class IGraphicsManager;
  class MaterialDescriptor;
  class UnlitMaterialDescriptor;
  class UnlitMaterial;
  class ITexture;

  class MaterialManager :
    public NonCopyable,
    public IDependencyResolvable
  {
  public:
    MaterialManager();
    ~MaterialManager();

    void resolveDependencies(DependencyContainer& container) override;

    SharedPtr<IMaterial> createMaterial(
      const String& key,
      const String& materialDescriptorKey
    );

    SharedPtr<IMaterial> createMaterial(
      const String& key, SharedPtr<MaterialDescriptor> descriptor
    );

    SharedPtr<UnlitMaterial> createUnlitMaterial(
      const String& key,
      SharedPtr<UnlitMaterialDescriptor> descriptor
    );

    SharedPtr<IMaterial> get(const String& key) const;
    bool contains(const String& key) const;

    const UnorderedMap<String, SharedPtr<IMaterial>>& getMaterials() const;
    void clear();

  private:
    SharedPtr<AssetManager> m_assetManager;
    SharedPtr<IGraphicsManager> m_graphicsManager;
    UnorderedMap<String, SharedPtr<IMaterial>> m_materials;

    SharedPtr<ITexture> createTextureFromImageKey(
      const String& imageKey
    ) const;
  };
}
