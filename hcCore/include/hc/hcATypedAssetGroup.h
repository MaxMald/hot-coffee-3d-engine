#pragma once

#include "hc/hcIAssetGroup.h"
#include "hc/hcAsset.h"

namespace hc
{
  /**
   * @brief Manages a collection of assets of a specific type using smart
   * pointers.
   *
   * @tparam T Asset type, must derive from Asset.
   */
  template <typename T>
  class ATypedAssetGroup : public IAssetGroup
  {
    static_assert(std::is_base_of_v<Asset, T>, "T must derive from Asset");

  public:
    ATypedAssetGroup() = default;
    virtual ~ATypedAssetGroup() = default;

    /**
     * @brief Removes an asset by key.
     *
     * @param key The unique key of the asset.
     */
    void remove(const String& key) override;

    /**
     * @brief Clears all assets from the group.
     */
    void clear() override;

    /**
     * @brief Gets the number of assets in the group.
     *
     * @return Asset count.
     */
    SizeT size() const override;

    /**
     * @brief Checks if the group contains an asset with the given key.
     *
     * @param key The unique key of the asset.
     *
     * @return True if the asset exists, false otherwise.
     */
    bool contains(const String& key) const override;

    /**
    * @brief Adds an asset to the group.
    *
    * @param asset Shared pointer to the asset.
    */
    void add(const String& key, const SharedPtr<T>& asset);

    /**
     * @brief Retrieves an asset by key.
     *
     * @param key The unique key of the asset.
     *
     * @return Shared pointer to the asset, or nullptr if not found.
     */
    SharedPtr<T> get(const String& key) const;

    /**
     * @brief Provides const access to the internal asset map.
     *
     * @return Const reference to the asset map.
     */
    const UnorderedMap<String, SharedPtr<T>>& getAll() const;

  private:
    UnorderedMap<String, SharedPtr<T>> m_assets;
  };

  template <typename T>
  void ATypedAssetGroup<T>::add(const String& key, const SharedPtr<T>& asset)
  {
    if (!asset)
      return;

    m_assets.emplace(key, asset);
  }

  template <typename T>
  SharedPtr<T> ATypedAssetGroup<T>::get(const String& key) const
  {
    auto it = m_assets.find(key);
    if (it != m_assets.end())
      return it->second;

    return nullptr;
  }

  template <typename T>
  void ATypedAssetGroup<T>::remove(const String& key)
  {
    m_assets.erase(key);
  }

  template <typename T>
  void ATypedAssetGroup<T>::clear()
  {
    m_assets.clear();
  }

  template <typename T>
  SizeT ATypedAssetGroup<T>::size() const
  {
    return m_assets.size();
  }

  template <typename T>
  bool ATypedAssetGroup<T>::contains(const String& key) const
  {
    return m_assets.find(key) != m_assets.end();
  }

  template <typename T>
  const UnorderedMap<String, SharedPtr<T>>& ATypedAssetGroup<T>::getAll() const
  {
    return m_assets;
  }
}
