#include "hc/assets/materialDescriptor/hcMaterialDescriptorAssetManager.h"

namespace hc
{
  MaterialDescriptorAssetManager::MaterialDescriptorAssetManager() :
    m_loadedMaterialDescriptors(),
    m_defaultMaterialDescriptor(MakeShared<AMaterialDescriptor>(""))
  {}

  SharedPtr<AMaterialDescriptor> MaterialDescriptorAssetManager::load(
    const Path& path
  )
  {
    // TODO : Implement actual loading logic from binary file
    return nullptr;
  }

  SharedPtr<AMaterialDescriptor> MaterialDescriptorAssetManager::get(
    const Path& path
  ) const
  {
    auto it = m_loadedMaterialDescriptors.find(path);
    if (it != m_loadedMaterialDescriptors.end())
      return it->second;

    throw RuntimeErrorException(
      String::Format(
        "Material descriptor not loaded at path: %s",
        path.toGenericString().c_str()
      )
    );
  }

  bool MaterialDescriptorAssetManager::isLoaded(const Path& path) const
  {
    return m_loadedMaterialDescriptors.find(path) != m_loadedMaterialDescriptors.end();
  }

  void MaterialDescriptorAssetManager::clear()
  {
    m_loadedMaterialDescriptors.clear();
  }

  void MaterialDescriptorAssetManager::getAllLoadedAssets(
    Vector<SharedPtr<AMaterialDescriptor>>& outAssets
  ) const
  {
    outAssets.clear();
    for (const auto& pair : m_loadedMaterialDescriptors)
      outAssets.push_back(pair.second);
  }

  SizeT MaterialDescriptorAssetManager::size() const
  {
    return m_loadedMaterialDescriptors.size();
  }

  SharedPtr<AMaterialDescriptor> MaterialDescriptorAssetManager::getDefault() const
  {
    return m_defaultMaterialDescriptor;
  }
}
