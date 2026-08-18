#pragma once

#include "hc/utilities/hcTextureFormatType.h"
#include "hc/utilities/hcColorSpaceType.h"
#include "hc/assets/hcAsset.h"

namespace hc
{
  /**
   * @brief Represents a image asset in the engine.
   */
  class HC_CORE_EXPORT Image : public Asset
  {
  public:
    Image(
      const Path& path,
      UInt32 width,
      UInt32 height,
      textureFormatType::Type format,
      colorSpaceType::Type colorSpace,
      BufferByte&& buffer
    );

    virtual ~Image();

    /**
     * @brief Gets the width of the image in pixels.
     * 
     * @return Image width.
     */
    UInt32 getWidth() const;

    /**
     * @brief Gets the height of the image in pixels.
     * 
     * @return Image height.
     */
    UInt32 getHeight() const;

    /**
     * @brief Gets the texture format of the image.
     *
     * @return Image texture format.
     */
    textureFormatType::Type getFormat() const;

    /**
     * @brief Gets the color space of the image.
     *
     * @return Image color space.
     */
    colorSpaceType::Type getColorSpace() const;

    /**
     * @brief Sets the color space of the image.
     *
     * @param colorSpace The new color space to set for the image.
     */
    void setColorSpace(colorSpaceType::Type colorSpace);

    /**
     * @brief Gets the buffer containing the image's image data.
     *
     * @return Reference to the image's image data buffer.
     */
    BufferByte& getBuffer();

    /**
     * @brief Gets the buffer containing the image's image data.
     *
     * @return Const reference to the image's image data buffer.
     */
    const BufferByte& getBuffer() const;

  private:
    UInt32 m_width;
    UInt32 m_height;
    textureFormatType::Type m_format;
    colorSpaceType::Type m_colorSpace;
    BufferByte m_data;
  };
}
