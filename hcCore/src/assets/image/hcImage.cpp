#include "hc/assets/image/hcImage.h"

namespace hc
{
  Image::Image(
    const Path& path,
    UInt32 width,
    UInt32 height,
    UInt32 channels,
    BufferByte&& buffer
  ) :
    Asset(path),
    m_width(width),
    m_height(height),
    m_channels(channels),
    m_data(std::move(buffer))
  {
  }

  Image::~Image()
  {
  }

  UInt32 Image::getWidth() const
  {
    return m_width;
  }

  UInt32 Image::getHeight() const
  {
    return m_height;
  }

  UInt32 Image::getChannels() const
  {
    // TODO
    //
    // Images should use color format type instead of channels.

    return m_channels;
  }

  BufferByte& Image::getBuffer()
  {
    return m_data;
  }

  const BufferByte& Image::getBuffer() const
  {
    return m_data;
  }
}
