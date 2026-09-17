#pragma once

#include "hc/graphics/resource/frameBuffer/hcIFrameBuffer.h"

namespace hc
{
  /**
   * @brief Interface for depth buffer resources used in rendering.
   *
   * A depth buffer is a specialized framebuffer that contains a depth texture attachment.
   * It is used to store depth information during rendering. It does not have color
   * attachments, and its primary purpose is to facilitate depth testing and depth-based
   * effects.
   *
   * @warning Do not call the getColorTexture() method on this interface, as it is not
   * applicable for depth buffers.
   */
  class HC_CORE_EXPORT IDepthBuffer : public IFrameBuffer
  {
  public:
    virtual ~IDepthBuffer();

    /**
     * @brief Gets the color texture attached to this depth buffer.
     *
     * @warning This method is not applicable for depth buffers and will throw an
     * exception if called. Depth buffers do not have color attachments.
     *
     * @return Reference to the color texture (not applicable).
     */
    virtual ITexture& getColorTexture() = 0;

    /**
     * @brief Gets the depth texture attached to this depth buffer.
     *
     * @return Reference to the depth texture.
     */
    virtual ITexture& getDepthTexture() = 0;

  protected:
    IDepthBuffer();
  };
}
