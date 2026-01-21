#pragma once

#include "hc/hcMaterialDescriptor.h"
#include "hc/hcColor.h"

namespace hc
{
  /**
   * @brief Descriptor for an unlit material asset.
   */
  class HC_CORE_EXPORT UnlitMaterialDescriptor :
    public MaterialDescriptor
  {
  public:
    UnlitMaterialDescriptor();
    virtual ~UnlitMaterialDescriptor();

    /**
     * @brief Sets the color property of the unlit material.
     *
     * @param color The color to set.
     */
    void setColor(const Color& color);

    /**
     * @brief Gets the color property of the unlit material.
     *
     * @return The current color value.
     */
    const Color& getColor() const;

    /**
     * @brief Sets the key for the main image used by the unlit material.
     * 
     * @param key The asset key or identifier for the main image.
     */
    void setMainImageKey(const String& key);

    /**
     * @brief Gets the key for the main image used by the unlit material.
     * 
     * @return The asset key or identifier for the main image.
     */
    const String& getMainImageKey() const;

  private:
    Color m_color;
    String m_mainImageKey;
  };
}
