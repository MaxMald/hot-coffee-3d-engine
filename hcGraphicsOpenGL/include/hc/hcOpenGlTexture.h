#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include <GL/glew.h>

namespace hc
{
  class Image;

  class OpenGlTexture : public ITexture
  {
  public:
    OpenGlTexture(SharedPtr<Image> image);
    ~OpenGlTexture() override;

    const Id& getId() const override;
    UInt32 getWidth() const override;
    UInt32 getHeight() const override;
    void bind(UInt32 slot = 0) const override;
    void unbind(UInt32 slot = 0) const override;
    bool isValid() const override;
    void destroy() override;
    SharedPtr<Image> getImage() override;
    void* getNativeHandle() const override;

    GLuint getTextureId() const;

  private:
    Id m_id;
    SharedPtr<Image> m_image;
    GLuint m_textureId;
    UInt32 m_width;
    UInt32 m_height;
    UInt32 m_channels;
    bool m_created;
  };
}
