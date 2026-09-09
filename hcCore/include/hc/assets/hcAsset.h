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
    Path path;

    virtual ~Asset() = default;

    /**
     * Gets the unique identifier of the asset.
     *
     * @return The asset's identifier
     */
    const UUID& getUUID() const;

  protected:
    UUID m_uuid;

    /**
     * Default constructor for an asset.
     */
    Asset();

    /**
     * Constructs an asset with the given path.
     *
     * @param path The file path to the asset resource
     */
    Asset(const Path& path);
  };
}
