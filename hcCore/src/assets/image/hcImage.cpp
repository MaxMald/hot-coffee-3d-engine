#include "hc/assets/image/hcImage.h"

namespace hc
{
  Image::Image(
    const Path& path,
    UInt32 width,
    UInt32 height,
    textureFormatType::Type format,
    colorSpaceType::Type colorSpace,
    BufferByte&& buffer
  ) :
    Asset(path),
    m_width(width),
    m_height(height),
    m_format(format),
    m_colorSpace(colorSpace),
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

  textureFormatType::Type Image::getFormat() const
  {
    return m_format;
  }

  colorSpaceType::Type Image::getColorSpace() const
  {
    return m_colorSpace;
  }

  void Image::setColorSpace(colorSpaceType::Type colorSpace)
  {
    m_colorSpace = colorSpace;
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
