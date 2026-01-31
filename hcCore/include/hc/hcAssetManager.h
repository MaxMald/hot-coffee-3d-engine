#pragma once

#include "hc/hcCorePrerequisites.h"
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
     * @brief Loads an asset of type T using the appropriate asset loader. The
     * key of the asset is derived from the path.
     *
     * @tparam T Asset type to load.
     * 
     * @param path Path to the asset resource.
     * 
     * @return Shared pointer to the loaded asset, or nullptr on failure.
     */
    template<typename T>
    SharedPtr<T> load(const Path& path);

    /**
     * @brief Loads an asset of type T directly from the specified path using the
     * appropriate asset loader.
     *
     * This method does not add the loaded asset to any asset group or cache. It
     * simply loads the asset from disk and returns it.
     *
     * @tparam T Asset type to load.
     * 
     * @param path Path to the asset resource.
     * 
     * @return Shared pointer to the loaded asset, or nullptr on failure.
     */
    template<typename T>
    SharedPtr<T> loadDirect(const Path& path);

    /**
     * @brief Retrieves an asset of type T by its unique key.
     * 
     * @tparam T Asset type.
     * 
     * @param key Unique identifier for the asset.
     * 
     * @return Shared pointer to the asset, or nullptr if not found.
     */
    template<typename T>
    SharedPtr<T> get(const String& key);

    /**
     * @brief Checks if an asset of type T exists for the given key.
     * 
     * @tparam T Asset type.
     * 
     * @param key Unique identifier for the asset.
     * 
     * @return True if the asset exists, false otherwise.
     */
    template<typename T>
    bool contains(const String& key);

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
    if (contains<T>(key))
      return get<T>(key);

    SharedPtr<T> loadedAsset = loadDirect<T>(path);

    if (!loadedAsset)
      return nullptr;

    AssetGroup<T>& assetGroup = getGroup<T>();
    assetGroup.add(key, loadedAsset);

    LogService::Message(
      String::Format(
        "Asset of type %s loaded and added to group with key: %s",
        typeid(T).name(),
        key.c_str()
      )
    );

    return loadedAsset;
  }

  template <typename T>
  inline SharedPtr<T> AssetManager::load(const Path& path)
  {
    String key = path.generic_string();
    return load<T>(key, path);
  }

  template<typename T>
  inline SharedPtr<T> AssetManager::loadDirect(const Path& path)
  {
    TypeIndex typeIdx(typeid(T));

    if(m_assetLoaders.find(typeIdx) == m_assetLoaders.end())
      throw RuntimeErrorException(
        String::Format("No asset loader registered for type %s.", typeid(T).name())
      );

    auto loader = static_cast<IAssetLoader<T>*>(m_assetLoaders[typeIdx].get());
    SharedPtr<T> loadedAsset = loader->load(path);

    if (loadedAsset)
    {
      SharedPtr<Asset> genericAsset = std::static_pointer_cast<Asset>(loadedAsset);
      if (genericAsset)
        genericAsset->m_path = path;

      LogService::Message(
        String::Format("Successfully loaded asset of type %s from path: %s",
          typeid(T).name(),
          path.generic_string().c_str()
        )
      );
    }
    else
    {
      LogService::Error(
        String::Format("Failed to load asset of type %s from path: %s",
          typeid(T).name(),
          path.generic_string().c_str()
        )
      );
    }

    return loadedAsset;
  }

  template<typename T>
  inline SharedPtr<T> AssetManager::get(const String& key)
  {
    AssetGroup<T>& assetGroup = getGroup<T>();
    return assetGroup.get(key);
  }

  template<typename T>
  inline bool AssetManager::contains(const String& key)
  {
    AssetGroup<T>& assetGroup = getGroup<T>();
    return assetGroup.contains(key);
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
