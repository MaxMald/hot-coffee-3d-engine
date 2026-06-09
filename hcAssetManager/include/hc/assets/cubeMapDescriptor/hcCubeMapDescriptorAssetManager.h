#pragma once

#include "hc/hcAssetManagerPrerequisites.h"

namespace hc
{
  class CubeMapDescriptorAssetManager : public ICubeMapDescriptorAssetManager
  {
  public:
    CubeMapDescriptorAssetManager();
    ~CubeMapDescriptorAssetManager() override = default;

    SharedPtr<CubeMapDescriptor> load(const Path& path) override;
    SharedPtr<CubeMapDescriptor> get(const Path& path) const override;
    bool isLoaded(const Path& path) const override;
    void clear() override;
    void getAllLoadedAssets(Vector<SharedPtr<CubeMapDescriptor>>& outAssets) const override;
    SizeT size() const override;

  private:
    UnorderedMap<Path, SharedPtr<CubeMapDescriptor>> m_loadedCubeMapDescriptors;
  };
}
