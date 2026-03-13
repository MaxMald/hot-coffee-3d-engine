#pragma once

#include "hc/assets/hcIAssetLoader.h"
#include "hc/assets/hcAsset.h"

namespace hc
{
  /**
   * @brief Abstract base class for type-specific asset loaders.
   *
   * @tparam T Asset type to be loaded, must derive from Asset.
   */
  template<typename T>
  class ATypedAssetLoader : public IAssetLoader
  {
    static_assert(std::is_base_of_v<Asset, T>, "T must derive from Asset");

  public:
    virtual ~ATypedAssetLoader() = default;

    /**
     * @brief Loads an asset from the specified path.
     *
     * @param path Path to the asset resource.
     * @return Shared pointer to the loaded asset, or nullptr on failure.
     */
    virtual SharedPtr<T> load(const Path& path) = 0;

  protected:
    ATypedAssetLoader() = default;
  };
}
