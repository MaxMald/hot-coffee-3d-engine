#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * Abstract base class for managing typed assets.
   *
   * Provides a generic interface for loading, retrieving, and managing
   * assets of a specific type. Implementations should handle caching and
   * lifecycle management of assets.
   *
   * @tparam AssetType The type of asset to be managed
   */
  template<typename AssetType>
  class ATypedAssetManager
  {
  public:
    virtual ~ATypedAssetManager() = default;

    /**
     * Loads an asset from the specified path.
     *
     * If the asset is already loaded, returns the existing instance.
     *
     * @param path The file path to the asset
     *
     * @return A shared pointer to the loaded asset, or nullptr on failure
     */
    virtual SharedPtr<AssetType> load(const Path& path) = 0;

    /**
     * Retrieves a previously loaded asset.
     *
     * @param path The file path to the asset
     *
     * @return A shared pointer to the asset if loaded, nullptr otherwise
     */
    virtual SharedPtr<AssetType> get(const Path& path) const = 0;

    /**
     * Checks if an asset at the specified path is currently loaded.
     *
     * @param path The file path to check
     *
     * @return true if the asset is loaded, false otherwise
     */
    virtual bool isLoaded(const Path& path) const = 0;

    /**
     * Clears all loaded assets from the manager.
     */
    virtual void clear() = 0;

  protected:
    ATypedAssetManager() = default;
  };
}
