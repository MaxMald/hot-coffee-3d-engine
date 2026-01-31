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
    Asset();
    virtual ~Asset();

    /**
     * @brief Gets the file or resource path of the asset.
     * 
     * @return The asset's path.
     */
    const Path& getPath() const;

  protected:
    Path m_path;

    friend class AssetManager;
  };
}
