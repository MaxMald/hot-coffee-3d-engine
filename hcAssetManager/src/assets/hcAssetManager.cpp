#include "hc/assets/hcAssetManager.h"

namespace hc
{
  AssetManager::AssetManager() :
    m_materialDescriptorAssetManager(),
    m_modelAssetManager(m_materialDescriptorAssetManager),
    m_imageAssetManager()
  {
  }

  IModelAssetManager& AssetManager::getModelAssetManager()
  {
    return m_modelAssetManager;
  }

  IImageAssetManager& AssetManager::getImageAssetManager()
  {
    return m_imageAssetManager;
  }

  IMaterialDescriptorAssetManager& AssetManager::getMaterialDescriptorAssetManager()
  {
    return m_materialDescriptorAssetManager;
  }

  void AssetManager::clear()
  {
    m_materialDescriptorAssetManager.clear();
    m_modelAssetManager.clear();
    m_imageAssetManager.clear();
  }

  void AssetManager::destroy()
  {
    clear();
  }
}
