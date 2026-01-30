#include "hc/hcOpenGlTexture.h"
#include "hc/hcImage.h"

namespace hc
{
  OpenGlTexture::OpenGlTexture(SharedPtr<Image> image) :
    m_textureId(0),
    m_width(0),
    m_height(0),
    m_channels(0),
    m_created(false)
  {
    if (!image)
      return;

    m_width = image->getWidth();
    m_height = image->getHeight();
    m_channels = image->getChannels();

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format = (m_channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(
      GL_TEXTURE_2D, 0, format,
      static_cast<Int32>(m_width),
      static_cast<Int32>(m_height),
      0, format,
      GL_UNSIGNED_BYTE, image->getBuffer().data()
    );

    glBindTexture(GL_TEXTURE_2D, 0);

    m_created = true;
  }

  OpenGlTexture::~OpenGlTexture()
  {
    destroy();
  }

  UInt32 OpenGlTexture::getWidth() const
  {
    return m_width;
  }

  UInt32 OpenGlTexture::getHeight() const
  {
    return m_height;
  }

  void OpenGlTexture::bind(UInt32 slot) const
  {
    if (!m_created)
      return;

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
  }

  void OpenGlTexture::unbind(UInt32 slot) const
  {
    if (!m_created)
      return;

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  bool OpenGlTexture::isValid() const
  {
    return m_created;
  }

  void OpenGlTexture::destroy()
  {
    if (m_created)
    {
      glDeleteTextures(1, &m_textureId);
      m_textureId = 0;
      m_created = false;
    }
  }

  void* OpenGlTexture::getNativeHandle() const
  {
    // Return the address of the GLuint as a void* for interoperability
    return reinterpret_cast<void*>(static_cast<uintptr_t>(m_textureId));
  }

  GLuint OpenGlTexture::getTextureId() const
  {
    return m_textureId;
  }
}
