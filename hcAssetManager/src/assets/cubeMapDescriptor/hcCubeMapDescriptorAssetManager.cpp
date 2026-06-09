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

      BinaryReader reader(file);

      SharedPtr<CubeMapDescriptor> cubeMapDescriptor = MakeShared<CubeMapDescriptor>();
      cubeMapDescriptor->deserialize(reader);
      cubeMapDescriptor->setPath(path);

      Path basePath = path.parent_path();

      cubeMapDescriptor->rightImagePath = AssetPath::ToAbsolute(
        cubeMapDescriptor->rightImagePath, basePath
      ).generic_string();
      cubeMapDescriptor->leftImagePath = AssetPath::ToAbsolute(
        cubeMapDescriptor->leftImagePath, basePath
      ).generic_string();
      cubeMapDescriptor->topImagePath = AssetPath::ToAbsolute(
        cubeMapDescriptor->topImagePath, basePath
      ).generic_string();
      cubeMapDescriptor->bottomImagePath = AssetPath::ToAbsolute(
        cubeMapDescriptor->bottomImagePath, basePath
      ).generic_string();
      cubeMapDescriptor->backImagePath = AssetPath::ToAbsolute(
        cubeMapDescriptor->backImagePath, basePath
      ).generic_string();
      cubeMapDescriptor->frontImagePath = AssetPath::ToAbsolute(
        cubeMapDescriptor->frontImagePath, basePath
      ).generic_string();

      m_loadedCubeMapDescriptors[path] = cubeMapDescriptor;
      return cubeMapDescriptor;
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to load CubeMapDescriptor from path: " + path.string() + ". Exception: " + e.what()
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
