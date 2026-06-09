#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  // TODO
  //
  // Assets are not no-copyable, that is they can be copied. Because of this, we should
  // create a new ID when copied. Also, consider using the utilities' UUID class instead
  // of ID.

  /**
   * Base class for all engine assets.
   *
   * Represents a loadable resource with a unique identifier and file path.
   * Assets are managed by the AssetManager and cannot be copied.
   */
  class HC_CORE_EXPORT Asset
  {
  public:
    virtual ~Asset() = default;

    /**
     * Gets the file or resource path of the asset.
     *
     * @return The asset's path
     */
    const Path& getPath() const;

    /**
     * Sets the file or resource path of the asset.
     *
     * @param path The new path to set for the asset
     */
    void setPath(const Path& path);

    /**
     * Gets the unique identifier of the asset.
     *
     * @return The asset's identifier
     */
    Id getId() const;

  protected:
    Path m_path;

    /**
     * Constructs an asset with the given path.
     *
     * @param path The file path to the asset resource
     */
    Asset(const Path& path);

  private:
    Id m_id;
  };
}
