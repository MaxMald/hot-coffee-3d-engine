#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIDependencyResolvable.h"
#include "hc/hcAssetGroup.h"
#include "hc/hcIAssetLoader.h"

namespace hc
{
  /**
   * @brief Manages all asset groups in the engine, providing type-safe access.
   */
  class AssetManager :
    public NonCopyable,
    public IDependencyResolvable
  {
  public:
    AssetManager();
    ~AssetManager();

    /**
     * @brief Resolves dependencies for the asset manager.
     */
    void resolveDependencies(DependencyContainer& container) override;

    /**
     * @brief Loads an asset of type T using the appropriate asset loader.
     * 
     * @tparam T Asset type to load.
     * 
     * @param key Unique identifier for the asset.
     * @param path Path to the asset resource.
     * 
     * @return Shared pointer to the loaded asset, or nullptr on failure.
     */
    template<typename T>
    SharedPtr<T> load(const String& key, const Path& path);

    /**
     * @brief Gets the asset group for the specified type. If the group does not
     * exist, it is created.
     * 
     * @tparam T Asset type.
     * 
     * @return Reference to the asset group for type T.
     */
    template<typename T>
    AssetGroup<T>& getGroup();

    /**
     * @brief Clears all asset groups and their contents.
     */
    void clear();

  private:
    UnorderedMap<TypeIndex, SharedPtr<void>> m_assetGroups;
    UnorderedMap<TypeIndex, SharedPtr<void>> m_assetLoaders;

    template<typename T>
    void createGroup();
  };

  template<typename T>
  void AssetManager::createGroup()
  {
    TypeIndex typeIdx(typeid(T));
    if (m_assetGroups.find(typeIdx) == m_assetGroups.end())
      m_assetGroups[typeIdx] = MakeShared<AssetGroup<T>>();
  }

  template<typename T>
  inline SharedPtr<T> AssetManager::load(const String& key, const Path& path)
  {
    TypeIndex typeIdx(typeid(T));

    if(m_assetLoaders.find(typeIdx) == m_assetLoaders.end())
      throw RuntimeErrorException(
        String::Format("No asset loader registered for type %s.", typeid(T).name())
      );

    auto loader = static_cast<IAssetLoader<T>*>(m_assetLoaders[typeIdx].get());
    SharedPtr<T> loadedAsset = loader->load(path);

    if (!loadedAsset)
      return nullptr;

    AssetGroup<T>& assetGroup = getGroup<T>();
    assetGroup.add(key, loadedAsset);

    return loadedAsset;
  }

  template<typename T>
  AssetGroup<T>& AssetManager::getGroup()
  {
    TypeIndex typeIdx(typeid(T));
    if (m_assetGroups.find(typeIdx) == m_assetGroups.end())
      createGroup<T>();

    return *static_cast<AssetGroup<T>*>(m_assetGroups[typeIdx].get());
  }
}
