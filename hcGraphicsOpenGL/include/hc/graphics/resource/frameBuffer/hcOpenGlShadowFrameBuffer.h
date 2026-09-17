#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class OpenGlShadowFrameBuffer
  {
  public:
    OpenGlShadowFrameBuffer();
    ~OpenGlShadowFrameBuffer();

    /**
     * @brief Initializes the shadow framebuffer with the specified width, height, and
     * number of layers.
     *
     * @param width The width of the shadow framebuffer.
     * @param height The height of the shadow framebuffer.
     * @param numLayers The number of layers in the shadow framebuffer.
     */
    void initialize(UInt32 width, UInt32 height, UInt32 numLayers);

    /**
     * @brief Returns the width of the shadow framebuffer.
     *
     * @returns The width of the shadow framebuffer.
     */
    UInt32 getWidth() const;

    /**
     * @brief Returns the height of the shadow framebuffer.
     *
     * @returns The height of the shadow framebuffer.
     */
    UInt32 getHeight() const;

    /**
     * @brief Returns the number of layers in the shadow framebuffer.
     *
     * @returns The number of layers in the shadow framebuffer.
     */
    UInt32 getNumLayers() const;

    /**
     * @brief Binds the shadow framebuffer for rendering for a specified layer. The layer
     * will be cleared before rendering to it. After rendering, you should call unbind()
     * to unbind the framebuffer.
     *
     * @param layer The index of the layer to bind for rendering.
     */
    void bind(UInt32 layer);

    /**
     * @brief Unbinds the shadow framebuffer, returning to the default framebuffer.
     */
    void unbind();

    /**
     * @brief Returns the OpenGL framebuffer ID of the shadow framebuffer.
     *
     * @returns The OpenGL framebuffer ID.
     */
    UInt32 getTextureArrayId() const;

    /**
     * @brief Destroys the shadow framebuffer and releases any associated resources.
     */
    void destroy();

  private:
    UInt32 m_frameBufferId;
    UInt32 m_depthTextureArrayId;
    UInt32 m_width;
    UInt32 m_height;
    UInt32 m_numLayers;
    bool m_initialized;

    void assertIsInitialized() const;
  };
}
