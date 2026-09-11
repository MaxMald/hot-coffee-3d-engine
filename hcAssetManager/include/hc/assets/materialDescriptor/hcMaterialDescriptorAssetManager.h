#pragma once

#include "hc/hcAssetManagerPrerequisites.h"

namespace hc
{
  /**
   * Concrete implementation of the material descriptor asset manager.
   *
   * Manages loading and caching of material descriptor assets from
   * JSON files. Provides a default material descriptor for fallback
   * purposes. Loaded descriptors are cached for reuse.
   */
  class MaterialDescriptorAssetManager : public IMaterialDescriptorAssetManager
  {
  public:
    /** Constructs the manager and initializes the default material */
    MaterialDescriptorAssetManager();

    /** Virtual destructor */
    virtual ~MaterialDescriptorAssetManager() = default;

    /**
     * Loads a material descriptor from the specified path.
     *
     * If the descriptor is already loaded, returns the cached instance.
     *
     * @param path The file path to the material descriptor asset
     * 
     * @return A shared pointer to the loaded descriptor, or nullptr on
     * failure
     */
    SharedPtr<MaterialDescriptor> load(const Path& path) override;

    /**
     * Retrieves a previously loaded material descriptor.
     *
     * @param path The file path to the material descriptor asset
     * 
     * @return A shared pointer to the descriptor if loaded
     */
    SharedPtr<MaterialDescriptor> get(const Path& path) const override;

    /**
     * Checks if a material descriptor is currently loaded.
     *
     * @param path The file path to check
     * 
     * @return true if the descriptor is loaded, false otherwise
     */
    bool isLoaded(const Path& path) const override;

    /**
     * Clears all loaded material descriptors from the cache.
     */
    void clear() override;

    /**
     * Retrieves all loaded material descriptors.
     *
     * @param outAssets A vector to be populated with shared pointers to all
     * loaded material descriptors. The vector will be cleared before being
     * populated.
     */
    void getAllLoadedAssets(Vector<SharedPtr<MaterialDescriptor>>& outAssets) const override;

    /**
     * Gets the number of material descriptors currently loaded in the manager.
     *
     * @return The number of loaded material descriptors
     */
    SizeT size() const override;

    /**
     * @copydoc IMaterialDescriptorAssetManager::getDefault
     */
    SharedPtr<MaterialDescriptor> getDefault() const override;

    /**
     * @copydoc IMaterialDescriptorAssetManager::save
     */
    void save(const Path& path, const MaterialDescriptor& descriptor) override;

  private:
    UnorderedMap<Path, SharedPtr<MaterialDescriptor>> m_loadedMaterialDescriptors;
    SharedPtr<MaterialDescriptor> m_defaultMaterialDescriptor;
  };
}
