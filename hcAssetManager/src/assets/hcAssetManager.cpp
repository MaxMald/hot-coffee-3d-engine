#include "hc/assets/hcAssetManager.h"

namespace hc
{
  AssetManager::AssetManager() :
    m_materialDescriptorAssetManager(),
    m_modelAssetManager(m_materialDescriptorAssetManager),
    m_imageAssetManager(),
    m_cubeMapDescriptorAssetManager(),
    m_rootPath()
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

  ICubeMapDescriptorAssetManager& AssetManager::getCubeMapDescriptorAssetManager()
  {
    return m_cubeMapDescriptorAssetManager;
  }

  void AssetManager::clear()
  {
    m_materialDescriptorAssetManager.clear();
    m_modelAssetManager.clear();
    m_imageAssetManager.clear();
    m_cubeMapDescriptorAssetManager.clear();
  }

  void AssetManager::destroy()
  {
    clear();
  }

  void AssetManager::setRootPath(const Path& rootPath)
  {
    m_rootPath = rootPath;
  }

  const Path& AssetManager::getRootPath() const
  {
    return m_rootPath;
  }

  bool AssetManager::hasRootPath() const
  {
    return !m_rootPath.empty();
  }
}
