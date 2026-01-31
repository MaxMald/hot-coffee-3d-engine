#include "hc/hcAssetManager.h"

#include "hc/hcIImageLoader.h"
#include "hc/hcIModelLoader.h"
#include "hc/hcMaterialDescriptor.h"
#include "hc/hcJsonDeserializerAssetLoader.h"

namespace hc
{
  AssetManager::AssetManager()
  {
    m_assetLoaders[typeid(MaterialDescriptor)] = 
      MakeShared<JsonDeserializerAssetLoader<MaterialDescriptor>>();
  }

  AssetManager::~AssetManager()
  {
    clear();
  }

  void AssetManager::resolveDependencies(DependencyContainer& container)
  {
    SharedPtr<IImageLoader> imageLoader = container.resolve<IImageLoader>();
    m_assetLoaders[typeid(Image)] = imageLoader;
    SharedPtr<IModelLoader> modelLoader = container.resolve<IModelLoader>();
    m_assetLoaders[typeid(Model)] = modelLoader;
  }

  void AssetManager::clear()
  {
    m_assetGroups.clear();
  }
}
