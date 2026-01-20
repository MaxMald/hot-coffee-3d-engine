#pragma once

#include "hc/hcAsset.h"
#include "hc/hcBuffer.h"

namespace hc
{
  /**
   * @brief Represents a texture asset in the engine.
   */
  class HC_CORE_EXPORT Texture :
    public Asset
  {
  public:
    Texture();
    virtual ~Texture();

    /**
     * @brief Gets the width of the texture in pixels.
     * 
     * @return Texture width.
     */
    UInt32 getWidth() const;

    /**
     * @brief Gets the height of the texture in pixels.
     * 
     * @return Texture height.
     */
    UInt32 getHeight() const;

    /**
     * @brief Gets the buffer containing the texture's image data.
     *
     * @return Reference to the texture's image data buffer.
     */
    BufferByte& getBuffer();

    /**
     * @brief Gets the buffer containing the texture's image data.
     *
     * @return Const reference to the texture's image data buffer.
     */
    const BufferByte& getBuffer() const;

  private:
    /**
     * @brief Initializes the texture with dimensions and image data.
     * 
     * @param width Texture width.
     * @param height Texture height.
     * @param buffer Buffer containing image data. Ownership is transferred.
     */
    void initialize(
      UInt32 width,
      UInt32 height,
      BufferByte&& buffer
    );

    UInt32 m_width;
    UInt32 m_height;
    BufferByte m_data;
  };
}
