#pragma once

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
      UInt32 channels,
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
     * @brief Gets the number of color channels in the image.
     * 
     * @return Number of channels.
     */
    UInt32 getChannels() const;

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
    UInt32 m_channels;
    BufferByte m_data;
  };
}
