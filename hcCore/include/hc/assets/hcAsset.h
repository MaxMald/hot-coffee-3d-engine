#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Base class for all engine assets.
   */
  class HC_CORE_EXPORT Asset :
    public NonCopyable
  {
  public:
    virtual ~Asset();

    /**
     * @brief Gets the file or resource path of the asset.
     * 
     * @return The asset's path.
     */
    const Path& getPath() const;

    /**
     * @brief Gets the unique identifier of the asset.
     * 
     * @return The asset's Id.
     */
    Id getId() const;

  protected:
    Path m_path;

    Asset();

  private:
    Id m_id;

    friend class AssetManager;
  };
}
