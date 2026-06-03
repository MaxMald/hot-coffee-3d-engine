#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc
{
  class OpenGlGBuffer : public IGBuffer
  {
  public:
    OpenGlGBuffer();
    ~OpenGlGBuffer() override;

    void initialize(UInt32 width, UInt32 height) override;
    void bind() override;
    void bindForReadingOnly() override;
    void bindForDrawingOnly() override;
    void unbind() override;
    void resize(UInt32 width, UInt32 height) override;
    virtual ITexture& getColorTexture() override;
    UInt32 getWidth() const override;
    UInt32 getHeight() const override;
    void clear(const Color& clearColor) override;
    bool isValid() const override;
    void destroy() override;
    void copyDepthTo(IFrameBuffer& destinationFrameBuffer) override;
    void destroySelf() override;

    void bindGTexturesForReading() override;
    const ITexture& getPositionAndDepth() const override;
    const ITexture& getNormalRoughness() const override;
    const ITexture& getAlbedoAlpha() const override;
    const ITexture& getMaterialParameters() const override;

  private:
    bool m_valid;
    UInt32 m_width;
    UInt32 m_height;
    UInt32 m_gBufferId;
    UInt32 m_depthStencilBufferId;
    OpenGlTexture m_positionAndDepthTexture;
    OpenGlTexture m_normalRoughnessTexture;
    OpenGlTexture m_albedoAlphaTexture;
    OpenGlTexture m_materialParametersTexture;

    void assertIsValid() const;
  };
}
