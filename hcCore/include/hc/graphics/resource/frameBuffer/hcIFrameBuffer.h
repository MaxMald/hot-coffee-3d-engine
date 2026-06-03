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
   * Calls the virtual destroy() method which handles both resource cleanup
   * and self-deletion in the same module where the object was allocated.
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
     * @brief Initializes the framebuffer with the specified dimensions. This method
     * must be called before using the framebuffer.
     *
     * You cannot initialize a framebuffer more than once without cleaning it up first.
     * If you need to change the size of an existing framebuffer, use the resize() method
     * instead.
     *
     * @param width The width in pixels.
     * @param height The height in pixels.
     */
    virtual void initialize(UInt32 width, UInt32 height) = 0;

    /**
     * @brief Binds this framebuffer as the current render target.
     */
    virtual void bind() = 0;

    /**
     * @brief Binds this framebuffer for reading only, without affecting the draw
     * bindings.
     */
    virtual void bindForReadingOnly() = 0;

    /**
     * @brief Binds this framebuffer for drawing only, without affecting the read
     * bindings.
     */
    virtual void bindForDrawingOnly() = 0;

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
     * @brief Clears the framebuffer's color buffer with the specified clear color.
     *
     * @param clearColor The color to clear the framebuffer with.
     */
    virtual void clear(const Color& clearColor) = 0;

    /**
     * @brief Checks if the framebuffer is initialized, valid and ready for use.
     * 
     * @return True if the framebuffer is initialize and valid, false otherwise.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief Destroys the framebuffer and releases all associated resources.
     */
    virtual void destroy() = 0;

    /**
     * @brief Copies the depth buffer from this framebuffer to the destination
     * framebuffer.
     *
     * The destination framebuffer must have a compatible depth attachment for this
     * operation to succeed.
     */
    virtual void copyDepthTo(IFrameBuffer& destinationFrameBuffer) = 0;

    /**
     * @brief Destroys the framebuffer and releases all associated resources.
     *
     * @warning This method should ONLY be called on heap-allocated objects managed
     * by FrameBufferPtr. Do not call destroySelf() on stack-allocated or member objects,
     * as it will cause undefined behavior and heap corruption.
     *
     * This method performs cleanup and self-deletion to ensure the object is destroyed in
     * the same module where it was allocated, maintaining heap consistency across DLL
     * boundaries.
     *
     * @see cleanup() for cleaning up member objects without deallocation.
     */
    virtual void destroySelf() = 0;

  protected:
    IFrameBuffer() = default;
  };
}
