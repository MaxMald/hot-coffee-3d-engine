#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"

namespace hc
{
  /**
   * @brief Base class for all engine assets.
   */
  class HC_CORE_EXPORT Asset :
    public NonCopyable
  {
  public:
    Asset(const String& key);
    virtual ~Asset();

    /**
     * @brief Gets the unique key of the asset.
     * 
     * @return The asset's key.
     */
    const String& getKey() const;

    /**
     * @brief Gets the file or resource path of the asset.
     * 
     * @return The asset's path.
     */
    const Path& getPath() const;

  protected:
    String m_key;
    Path m_path;
  };
}
