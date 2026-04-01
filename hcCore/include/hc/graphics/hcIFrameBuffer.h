#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ITexture;
  class IFrameBuffer;

  /**
   * @brief Custom deleter for IFrameBuffer that ensures proper cleanup across
   * DLL boundaries.
   *
   * Calls the virtual destroy() method to delete the object in the same heap
   * where it was allocated.
   */
  struct HC_CORE_EXPORT FrameBufferDeleter
  {
    void operator()(IFrameBuffer* frameBuffer) const;
  };

  /**
   * @brief Smart pointer type for managing IFrameBuffer lifetime safely across
   * DLL boundaries.
   */
  using FrameBufferPtr = UniquePtrWithDeleter<IFrameBuffer, FrameBufferDeleter>;

  /**
   * @brief Interface for framebuffer objects that support rendering to textures.
   */
  class HC_CORE_EXPORT IFrameBuffer : public NonCopyable
  {
  public:
    virtual ~IFrameBuffer() = default;

    /**
     * @brief Binds this framebuffer as the current render target.
     */
    virtual void bind() = 0;

    /**
     * @brief Unbinds this framebuffer, restoring the default render
     * target.
     */
    virtual void unbind() = 0;

    /**
     * @brief Resizes the framebuffer to the specified dimensions.
     * 
     * @param width The new width in pixels.
     * @param height The new height in pixels.
     */
    virtual void resize(UInt32 width, UInt32 height) = 0;

    /**
     * @brief Gets the color texture attached to this framebuffer.
     * 
     * @return Reference to the color texture.
     */
    virtual ITexture& getColorTexture() = 0;

    /**
     * @brief Gets the width of the framebuffer.
     * 
     * @return The width in pixels.
     */
    virtual UInt32 getWidth() const = 0;

    /**
     * @brief Gets the height of the framebuffer.
     * 
     * @return The height in pixels.
     */
    virtual UInt32 getHeight() const = 0;

    /**
     * @brief Checks if the framebuffer is valid and ready for use.
     * 
     * @return True if the framebuffer is valid, false otherwise.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief Destroys the framebuffer and releases all associated resources.
     */
    virtual void destroy() = 0;

  protected:
    IFrameBuffer() = default;
  };
}
