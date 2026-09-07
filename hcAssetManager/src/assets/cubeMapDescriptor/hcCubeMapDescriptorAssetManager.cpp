#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptorAssetManager.h"

#include <fstream>

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
      std::ifstream file(path, std::ios::binary);
      if (!file.is_open())
        throw IOException("Failed to open file for reading.");

      io::BinaryReader reader(file);

      SharedPtr<CubeMapDescriptor> descriptor = MakeShared<CubeMapDescriptor>();
      descriptor->deserialize(reader);
      descriptor->setPath(path);

      Path basePath = path.parentPath();
      descriptor->rightImagePath = descriptor->rightImagePath.toAbsolute(basePath);
      descriptor->leftImagePath = descriptor->leftImagePath.toAbsolute(basePath);
      descriptor->topImagePath = descriptor->topImagePath.toAbsolute(basePath);
      descriptor->bottomImagePath = descriptor->bottomImagePath.toAbsolute(basePath);
      descriptor->backImagePath = descriptor->backImagePath.toAbsolute(basePath);
      descriptor->frontImagePath = descriptor->frontImagePath.toAbsolute(basePath);

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
