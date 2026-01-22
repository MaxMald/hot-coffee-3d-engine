#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIDependencyResolvable.h"
#include "hc/hcIMaterial.h"

namespace hc
{
  class TextureManager;
  class AssetManager;
  class MaterialDescriptor;
  class UnlitMaterialDescriptor;
  class UnlitMaterial;
  class ITexture;

  /**
   * @brief Manages the creation, caching, and retrieval of materials.
   */
  class MaterialManager :
    public NonCopyable,
    public IDependencyResolvable
  {
  public:
    MaterialManager() = default;
    ~MaterialManager() = default;

    /**
     * @copydoc IDependencyResolvable::resolveDependencies
     */
    void resolveDependencies(DependencyContainer& container) override;

    /**
     * @brief Creates a material from a material descriptor file.
     *
     * @param materialDescriptorPath Path to the material descriptor file.
     *
     * @return Shared pointer to the created material.
     */
    SharedPtr<IMaterial> createMaterialFromFile(const Path& materialDescriptorPath);

    /**
     * @brief Creates a material from a material descriptor key.
     *
     * @param materialDescriptorKey Key identifying the material descriptor.
     *
     * @return Shared pointer to the created material.
     */
    SharedPtr<IMaterial> createMaterialFromDescriptor(const String& materialDescriptorKey);

    /**
     * @brief Creates a material from a material descriptor object.
     *
     * @param descriptor Shared pointer to the material descriptor.
     *
     * @return Shared pointer to the created material.
     */
    SharedPtr<IMaterial> createMaterialFromDescriptor(SharedPtr<MaterialDescriptor> descriptor);

    /**
     * @brief Retrieves a cached material by key.
     *
     * @param key Cache key for the material.
     *
     * @return Shared pointer to the material if found, nullptr otherwise.
     */
    SharedPtr<IMaterial> get(const String& key) const;

    /**
     * @brief Checks if a material exists in the cache.
     * 
     * @param key Cache key for the material.
     * 
     * @return True if the material exists, false otherwise.
     */
    bool contains(const String& key) const;

    /**
     * @brief Gets the map of all cached materials.
     * 
     * @return Const reference to the unordered map of cached materials.
     */
    const UnorderedMap<String, SharedPtr<IMaterial>>& getCacheMaterials() const;

    /**
     * @brief Clears all cached materials.
     */
    void clear();

  private:
    SharedPtr<AssetManager> m_assetManager;
    SharedPtr<TextureManager> m_textureManager;
    UnorderedMap<String, SharedPtr<IMaterial>> m_cacheMaterials;

    /**
     * @brief Generates a cache key for a given material descriptor.
     * 
     * @param descriptor Shared pointer to the material descriptor.
     * 
     * @return Generated cache key as a string.
     */
    String getCacheKey(SharedPtr<MaterialDescriptor> descriptor) const;

    /**
     * @brief Creates an unlit material from a descriptor and cache key.
     * 
     * @param cacheKey Cache key for the material.
     * @param descriptor Shared pointer to the material descriptor.
     * 
     * @return Shared pointer to the created unlit material.
     */
    SharedPtr<UnlitMaterial> createUnlitMaterial(
      const String& cacheKey,
      SharedPtr<MaterialDescriptor> descriptor
    );    
  };
}
