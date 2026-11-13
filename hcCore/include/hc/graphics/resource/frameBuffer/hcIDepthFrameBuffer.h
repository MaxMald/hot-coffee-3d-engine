#pragma once

#include "hc/graphics/hcGraphicsCommons.h"
#include "hc/graphics/resource/hcIGraphicResource.h"
#include "hc/graphics/resource/frameBuffer/hcFrameBufferCopySettings.h"

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
     * @param mipmapLevel The mipmap level of the depth texture to attach (default is 0).
     */
    virtual void initialize(const ITexture& depthTexture, UInt32 mipmapLevel = 0) = 0;

    /**
     * @brief Initializes the depth buffer, creating the underlying framebuffer object and
     * attaching a single layer of the given depth texture array as its initial depth
     * attachment.
     * @param depthTextureArray The texture array containing the depth layer.
     * @param layer The index of the layer within the texture array to attach.
     * @param mipmapLevel The mipmap level of the depth texture to attach (default is 0).
     */
    virtual void initialize(
      const textureArray::ITextureArray& depthTextureArray,
      UInt32 layer,
      UInt32 mipmapLevel = 0
    ) = 0;

    /**
     * @brief Replaces the current depth attachment with a standalone depth texture,
     * without affecting the underlying framebuffer object.
     * @note The depth buffer must already be initialized.
     * @param depthTexture The new texture to attach as the depth attachment.
     * @param mipmapLevel The mipmap level of the depth texture to attach (default is 0).
     */
    virtual void attachDepthTexture(
      const ITexture& depthTexture,
      UInt32 mipmapLevel = 0
    ) = 0;

    /**
     * @brief Replaces the current depth attachment with a single layer of a depth texture
     * array, without affecting the underlying framebuffer object.
     * @note The depth buffer must already be initialized.
     * @param depthTextureArray The texture array containing the depth layer.
     * @param layer The index of the layer within the texture array to attach.
     * @param mipmapLevel The mipmap level of the depth texture to attach (default is 0).
     */
    virtual void attachDepthTexture(
      const textureArray::ITextureArray& depthTextureArray,
      UInt32 layer,
      UInt32 mipmapLevel = 0
    ) = 0;

    /**
     * @brief Binds the depth buffer for the specified binding type.
     * @param bindingType The type of binding to perform (default is ReadDraw).
     */
    virtual void bind(
      frameBufferBindingType::Type bindingType = frameBufferBindingType::ReadDraw
    ) = 0;

    /**
     * @brief Unbinds the depth buffer from the specified binding type.
     * @param bindingType The type of binding to unbind (default is ReadDraw).
     */
    virtual void unbind(
      frameBufferBindingType::Type bindingType = frameBufferBindingType::ReadDraw
    ) = 0;

    /**
     * @brief Checks if the depth buffer is currently bound for the specified binding type.
     * @param bindingType The type of binding to check (Read, Draw, or ReadDraw).
     * @return True if the depth buffer is bound for the specified type; otherwise, false.
     */
    virtual bool isBound(frameBufferBindingType::Type bindingType) const = 0;

    /**
     * @brief Copies the depth buffer's content into another framebuffer.
     * @param destinationFrameBuffer The framebuffer to copy the depth data into.
     * @param copySettings The settings to use when copying the depth data.
     */
    virtual void copyDepthTo(
      IFrameBuffer& destinationFrameBuffer,
      const FrameBufferCopySettings& copySettings
    ) = 0;

    /**
     * @brief Clears the depth buffer's content to the specified depth value.
     * @param depthValue The depth value to clear the buffer to (default is 1.0).
     */
    virtual void clear() = 0;

  protected:
    IDepthFrameBuffer();
  };
}
