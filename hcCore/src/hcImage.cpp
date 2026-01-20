#include "hc/hcImage.h"

namespace hc
{
  Image::Image() :
    m_width(0),
    m_height(0),
    m_data(0)
  {
  }

  Image::~Image()
  {
  }

  void Image::initialize(
    UInt32 width,
    UInt32 height,
    BufferByte&& buffer
  )
  {
    m_width = width;
    m_height = height;
    m_data = std::move(buffer);
  }

  UInt32 Image::getWidth() const
  {
    return m_width;
  }

  UInt32 Image::getHeight() const
  {
    return m_height;
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
