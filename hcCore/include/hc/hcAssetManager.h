#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIDependencyResolvable.h"
#include "hc/hcAssetGroup.h"

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
  AssetGroup<T>& AssetManager::getGroup()
  {
    TypeIndex typeIdx(typeid(T));
    if (m_assetGroups.find(typeIdx) == m_assetGroups.end())
      createGroup<T>();

    return *static_cast<AssetGroup<T>*>(m_assetGroups[typeIdx].get());
  }
}
