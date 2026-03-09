#pragma once

#include "hc/hcAssetManagerPrerequisites.h"

namespace hc
{
  /**
   * Concrete implementation of the image asset manager.
   *
   * Manages loading and caching of image assets. Loaded images are
   * stored in memory and reused when requested multiple times.
   */
  class ImageAssetManager : public IImageAssetManager
  {
  public:
    /** Default constructor */
    ImageAssetManager() = default;

    /** Virtual destructor */
    virtual ~ImageAssetManager() = default;

    /**
     * Loads an image from the specified path.
     *
     * If the image is already loaded, returns the cached instance.
     *
     * @param path The file path to the image asset
     * 
     * @return A shared pointer to the loaded image, or nullptr on failure
     */
    SharedPtr<Image> load(const Path& path) override;

    /**
     * Retrieves a previously loaded image.
     *
     * @param path The file path to the image asset
     * 
     * @return A shared pointer to the image if loaded, nullptr otherwise
     */
    SharedPtr<Image> get(const Path& path) const override;

    /**
     * Checks if an image at the specified path is currently loaded.
     *
     * @param path The file path to check
     * 
     * @return true if the image is loaded, false otherwise
     */
    bool isLoaded(const Path& path) const override;

    /**
     * Gets the number of images currently loaded in the manager.
     *
     * @return The number of loaded images
     */
    SizeT size() const override;

    /**
     * Retrieves all loaded images managed by this asset manager.
     *
     * @param outAssets A vector to be populated with shared pointers to all
     * loaded images. Vector will be cleared before being populated.
     */
    void getAllLoadedAssets(Vector<SharedPtr<Image>>& outAssets) const override;

    /**
     * Clears all loaded images from the cache.
     */
    void clear() override;

  private:
    UnorderedMap<Path, SharedPtr<Image>> m_loadedImages;
  };
}
