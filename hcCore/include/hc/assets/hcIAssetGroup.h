#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class HC_CORE_EXPORT IAssetGroup : public NonCopyable
  {
  public:
    virtual ~IAssetGroup() = default;

    /**
     * @brief Gets the number of assets in the group.
     *
     * @return Asset count.
     */
    virtual SizeT size() const = 0;

    /**
     * @brief Checks if the group contains an asset with the given key.
     *
     * @param key The unique key of the asset.
     *
     * @return True if the asset exists, false otherwise.
     */
    virtual bool contains(const String& key) const = 0;

    /**
     * @brief Removes an asset by key.
     */
    virtual void remove(const String& key) = 0;

    /**
     * @brief Clears all assets from the group.
     */
    virtual void clear() = 0;

  protected:
    IAssetGroup() = default;
  };
}
