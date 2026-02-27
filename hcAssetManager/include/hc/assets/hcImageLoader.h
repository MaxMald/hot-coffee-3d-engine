#pragma once

#include <hc/assets/hcATypedAssetLoader.h>
#include "hc/hcAssetManagerPrerequisites.h"

namespace hc
{
  /**
   * @brief Asset loader for image resources.
   */
  class HC_ASSET_MANAGER_EXPORT ImageLoader : public ATypedAssetLoader<Image>
  {
  public:
    ImageLoader() = default;
    virtual ~ImageLoader() = default;

    /**
     * @brief Loads an image asset from the specified file path.
     *
     * @param filePath The path to the image file to load.
     * @return Shared pointer to the loaded image asset, or nullptr on failure.
     */
    SharedPtr<Image> load(const Path& filePath) override;
  };
}
