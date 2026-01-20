#include "hc/hcTexture.h"

namespace hc
{
  Texture::Texture(const String& key) :
    Asset(key),
    m_width(0),
    m_height(0),
    m_data(0)
  {
  }

  Texture::~Texture()
  {
  }

  UInt32 Texture::getWidth() const
  {
    return m_width;
  }

  UInt32 Texture::getHeight() const
  {
    return m_height;
  }

  BufferByte& Texture::getBuffer()
  {
    return m_data;
  }

  const BufferByte& Texture::getBuffer() const
  {
    return m_data;
  }

  void Texture::initialize(
    UInt32 width,
    UInt32 height,
    BufferByte&& buffer
  )
  {
    m_width = width;
    m_height = height;
    m_data = std::move(buffer);
  }
}
