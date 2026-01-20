#pragma once

#include "hc/hcAsset.h"
#include "hc/hcBuffer.h"

namespace hc
{
  class IImageLoader;

  /**
   * @brief Represents a image asset in the engine.
   */
  class HC_CORE_EXPORT Image :
    public Asset
  {
  public:
    Image();
    virtual ~Image();

    /**
     * @brief Initializes the image with dimensions and image data.
     *
     * @param width Image width.
     * @param height Image height.
     * @param buffer Buffer containing image data. Ownership is transferred.
     */
    void initialize(
      UInt32 width,
      UInt32 height,
      BufferByte&& buffer
    );

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
    BufferByte m_data;
  };
}
