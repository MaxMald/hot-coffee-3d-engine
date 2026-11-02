#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptorAssetManager.h"

namespace hc
{
  CubeMapDescriptorAssetManager::CubeMapDescriptorAssetManager() :
    m_loadedCubeMapDescriptors()
  {}

  SharedPtr<CubeMapDescriptor> CubeMapDescriptorAssetManager::load(const Path & path)
  {
    if (isLoaded(path))
      return m_loadedCubeMapDescriptors.at(path);

    try
    {
      String error;
      io::BinaryReader reader;
      if (!reader.prepare(path, error))
        throw IOException("Failed to prepare binary reader: " + error);

      SharedPtr<CubeMapDescriptor> descriptor = MakeShared<CubeMapDescriptor>();
      descriptor->deserialize(reader);
      descriptor->path = path;

      Path basePath = path.parentPath();
      descriptor->pXImagePath = descriptor->pXImagePath.toAbsolute(basePath);
      descriptor->nXImagePath = descriptor->nXImagePath.toAbsolute(basePath);
      descriptor->pYImagePath = descriptor->pYImagePath.toAbsolute(basePath);
      descriptor->nYImagePath = descriptor->nYImagePath.toAbsolute(basePath);
      descriptor->pZImagePath = descriptor->pZImagePath.toAbsolute(basePath);
      descriptor->nZImagePath = descriptor->nZImagePath.toAbsolute(basePath);

      m_loadedCubeMapDescriptors[path] = descriptor;
      return descriptor;
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to load CubeMapDescriptor from path: " + path.toString() + ". Exception: " + e.what()
      );
      return nullptr;
    }
  }

  SharedPtr<CubeMapDescriptor> CubeMapDescriptorAssetManager::get(const Path& path) const
  {
    if (isLoaded(path))
      return m_loadedCubeMapDescriptors.at(path);
    else
      return nullptr;
  }

  bool CubeMapDescriptorAssetManager::isLoaded(const Path& path) const
  {
    return m_loadedCubeMapDescriptors.find(path) != m_loadedCubeMapDescriptors.end();
  }

  void CubeMapDescriptorAssetManager::clear()
  {
    m_loadedCubeMapDescriptors.clear();
  }

  void CubeMapDescriptorAssetManager::getAllLoadedAssets(
    Vector<SharedPtr<CubeMapDescriptor>>&outAssets
  ) const
  {
    outAssets.clear();
    for (const auto& pair : m_loadedCubeMapDescriptors)
      outAssets.push_back(pair.second);
  }

  SizeT CubeMapDescriptorAssetManager::size() const
  {
    return m_loadedCubeMapDescriptors.size();
  }
}
