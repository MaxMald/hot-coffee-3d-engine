#pragma once

#include "hc/hcCoreCommons.h"
#include "hc/assets/hcAsset.h"

namespace hc
{
  /**
   * @brief Represents a image asset in the engine.
   *
   * @note RGB16F, RGBA16F and Depth24 texture formats are not valid for image data. Use
   * RGB32F, RGBA32F or Depth32F instead.
   */
  class HC_CORE_EXPORT Image : public Asset
  {
  public:
    /**
     * @brief Constructs an Image asset with the specified file path, dimensions, format,
     * color space, and pixel data buffer.
     *
     * @param path The file path to the image resource.
     * @param width The width of the image.
     * @param height The height of the image.
     * @param format The texture format of this image. RGB16F, RGBA16F and Depth24 are not
     * valid for image data.
     * @param colorSpace The color space of this image.
     * @param buffer The pixels data.
     */
    Image(
      const Path& path,
      UInt32 width,
      UInt32 height,
      textureFormatType::Type format,
      colorSpaceType::Type colorSpace,
      BufferByte&& buffer
    );

    /**
     * @brief Constructs an Image asset with the specified dimensions, format, color
     * space, and initial color.
     *
     * @param width The width of the image.
     * @param height The height of the image.
     * @param format The texture format of this image. RGB16F, RGBA16F and Depth24 are not
     * valid for image data.
     * @param colorSpace The color space of this image.
     * @param initColor The initial color to fill the image with (default is transparent
     * black).
     */
    Image(
      UInt32 width,
      UInt32 height,
      textureFormatType::Type format,
      colorSpaceType::Type colorSpace,
      const Color& initColor = Color(0, 0, 0, 0)
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


    /**
     * @brief Asserts that the provided texture format type is valid for an image.
     *
     * @param format The texture format type to check.
     *
     * @throws InvalidArgumentException if the format is undefined, out of range, or not
     * suitable for image data.
     */
    static inline void AssertValidTextureFormatType(textureFormatType::Type format)
    {
      if (
        format == textureFormatType::Undefined
        || format >= textureFormatType::Count
        || format == textureFormatType::RGB16F
        || format == textureFormatType::RGBA16F
        || format == textureFormatType::Depth24
        )
      {
        String formatStr = textureFormatType::ToString(format);
        throw InvalidArgumentException(
          String::Format(
            "Cannot create an Image. Invalid texture format type: %s", formatStr.c_str()
          )
        );
      }
    }
  };
}
