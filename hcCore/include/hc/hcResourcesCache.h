#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Generic cache for resources.
   *
   * Provides caching functionality for resources identified by a unique key.
   * Allows storing, retrieving, checking, and clearing cached resources.
   *
   * @tparam Key The type used as the unique identifier for resources.
   * @tparam ResourceType The type of the resource to be cached.
   */
  template <typename Key, typename ResourceType>
  class ResourcesCache
  {
  public:
    ResourcesCache() = default;
    virtual ~ResourcesCache() = default;

    /**
     * @brief Retrieves a cached resource by its unique identifier.
     *
     * @param id The unique identifier of the resource to retrieve.
     * 
     * @return Shared pointer to the cached resource if found, nullptr otherwise.
     */
    SharedPtr<ResourceType> getCachedResource(const Key& id) const;

    /**
     * @brief Checks if a resource with the given unique identifier is cached.
     *
     * @param id The unique identifier of the resource to check.
     * 
     * @return True if the resource is cached, false otherwise.
     */
    bool hasCachedResource(const Key& id) const;

    /**
     * @brief Caches a resource with its unique identifier.
     *
     * @param id The unique identifier of the resource to cache.
     * 
     * @param resource Shared pointer to the resource to cache.
     */
    void cacheResource(const Key& id, SharedPtr<ResourceType> resource);

    /**
     * @brief Retrieves all cached resources.
     *
     * @return Vector of shared pointers to all cached resources.
     */
    Vector<SharedPtr<ResourceType>> getCachedResources() const;

    /**
     * @brief Clears all cached resources from the cache.
     */
    void clearCache();

  private:
    /**
     * @brief Internal storage for cached resources, mapping unique identifiers
     * to their corresponding resource instances.
     */
    UnorderedMap<Key, SharedPtr<ResourceType>> m_cache;
  };

  template <typename Key, typename ResourceType>
  SharedPtr<ResourceType> ResourcesCache<Key, ResourceType>::getCachedResource(
    const Key& id
  ) const
  {
    auto it = m_cache.find(id);
    if (it != m_cache.end())
      return it->second;

    return SharedPtr<ResourceType>();
  }

  template <typename Key, typename ResourceType>
  bool ResourcesCache<Key, ResourceType>::hasCachedResource(
    const Key& id
  ) const
  {
    return m_cache.find(id) != m_cache.end();
  }

  template <typename Key, typename ResourceType>
  void ResourcesCache<Key, ResourceType>::cacheResource(
    const Key& id,
    SharedPtr<ResourceType> resource
  )
  {
    if (!resource)
      return;

    m_cache[id] = resource;
  }

  template<typename Key, typename ResourceType>
  inline Vector<SharedPtr<ResourceType>> ResourcesCache<Key, ResourceType>::getCachedResources() const
  {
    Vector<SharedPtr<ResourceType>> resources;
    for (const auto& pair : m_cache)
      resources.push_back(pair.second);
    return resources;
  }

  template <typename Key, typename ResourceType>
  void ResourcesCache<Key, ResourceType>::clearCache()
  {
    m_cache.clear();
  }
}
