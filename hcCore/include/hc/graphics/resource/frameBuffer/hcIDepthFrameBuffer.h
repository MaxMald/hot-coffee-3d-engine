#pragma once

#include "hc/graphics/resource/hcIGraphicResource.h"

namespace hc
{
  class ITexture;
  class IFrameBuffer;
}

namespace hc::graphics::resource::textureArray
{
  class ITextureArray;
}

namespace hc::graphics::resource::frameBuffer
{
  /**
   * @brief Represents a depth-only framebuffer resource used for depth rendering.
   *
   * @note An IDepthBuffer owns and manages the lifecycle of the underlying framebuffer
   * object, but does NOT own the depth texture (or texture array) attached to it.
   */
  class HC_CORE_EXPORT IDepthFrameBuffer : public IGraphicResource
  {
  public:
    /**
     * @brief Destroys the depth buffer, releasing the underlying framebuffer resource.
     */
    virtual ~IDepthFrameBuffer();

    /**
     * @brief Initializes the depth buffer, creating the underlying framebuffer object and
     * attaching the given texture as its initial depth attachment.
     * @param depthTexture The texture to use as the initial depth attachment.
     */
    virtual void initialize(const ITexture& depthTexture) = 0;

    /**
     * @brief Initializes the depth buffer, creating the underlying framebuffer object and
     * attaching a single layer of the given depth texture array as its initial depth
     * attachment.
     * @param depthTextureArray The texture array containing the depth layer.
     * @param layer The index of the layer within the texture array to attach.
     */
    virtual void initialize(
      const textureArray::ITextureArray& depthTextureArray,
      UInt32 layer
    ) = 0;

    /**
     * @brief Replaces the current depth attachment with a standalone depth texture,
     * without affecting the underlying framebuffer object.
     * @note The depth buffer must already be initialized.
     * @param depthTexture The new texture to attach as the depth attachment.
     */
    virtual void attachDepthTexture(const ITexture& depthTexture) = 0;

    /**
     * @brief Replaces the current depth attachment with a single layer of a depth texture
     * array, without affecting the underlying framebuffer object.
     * @note The depth buffer must already be initialized.
     * @param depthTextureArray The texture array containing the depth layer.
     * @param layer The index of the layer within the texture array to attach.
     */
    virtual void attachDepthTexture(
      const textureArray::ITextureArray& depthTextureArray,
      UInt32 layer
    ) = 0;

    /**
     * @brief Copies the depth buffer's content into another framebuffer.
     * @param destinationFrameBuffer The framebuffer to copy the depth data into.
     */
    virtual void copyDepthTo(IFrameBuffer& destinationFrameBuffer) = 0;

  protected:
    IDepthFrameBuffer();
  };
}
