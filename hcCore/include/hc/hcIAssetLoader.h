#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcAsset.h"

namespace hc
{
  /**
   * @brief Base class template for asset loaders.
   *
   * @tparam T Asset type to load, must derive from Asset.
   */
  template<typename T>
  class IAssetLoader
  {
    static_assert(std::is_base_of_v<Asset, T>, "T must derive from Asset");

  public:
    virtual ~IAssetLoader() = default;

    /**
     * @brief Loads an asset from the specified path.
     * 
     * @param path Path to the asset resource.
     * 
     * @return Shared pointer to the loaded asset, or nullptr on failure.
     */
    virtual SharedPtr<T> load(const Path& path) = 0;
  };
}
