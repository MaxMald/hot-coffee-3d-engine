#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc
{
  class OpenGlDepthBuffer : public IDepthBuffer
  {
  public:
    OpenGlDepthBuffer();
    ~OpenGlDepthBuffer() override;

    void initialize(UInt32 width, UInt32 height) override;
    void bind() override;
    void bindForReadingOnly() override;
    void bindForDrawingOnly() override;
    void unbind() override;
    void resize(UInt32 width, UInt32 height) override;
    ITexture& getColorTexture() override;
    UInt32 getWidth() const override;
    UInt32 getHeight() const override;
    void clear(const Color& clearColor) override;
    bool isValid() const override;
    void destroy() override;
    void copyDepthTo(IFrameBuffer& destinationFrameBuffer) override;
    void destroySelf() override;
    ITexture& getDepthTexture() override;

  private:
    OpenGlTexture m_depthTexture;
    UInt32 m_depthBufferId;
    UInt32 m_width;
    UInt32 m_height;
    bool m_valid;

    void assertIsValid() const;
  };
}
