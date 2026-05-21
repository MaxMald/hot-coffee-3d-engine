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
    void bindForWriting() override;
    void bindForReading() override;
    void clear() override;
    void unbind() override;
    bool isValid() const override;
    UInt32 getWidth() const override;
    UInt32 getHeight() const override;
    void resize(UInt32 width, UInt32 height) override;

    const ITexture& getPosition() const override;
    const ITexture& getNormalRoughness() const override;
    const ITexture& getAlbedoAlpha() const override;
    const ITexture& getMaterialParameters() const override;

    void destroy() override;

  private:
    bool m_valid;
    UInt32 m_width;
    UInt32 m_height;
    UInt32 m_gBufferId;
    UInt32 m_depthStencilBufferId;
    OpenGlTexture m_positionTexture;
    OpenGlTexture m_normalRoughnessTexture;
    OpenGlTexture m_albedoAlphaTexture;
    OpenGlTexture m_materialParametersTexture;

    void assertIsValid() const;
  };
}
