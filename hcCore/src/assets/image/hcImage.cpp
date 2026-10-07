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
    if (width == 0)
      throw InvalidArgumentException("Image width cannot be zero.");
    if (height == 0)
      throw InvalidArgumentException("Image height cannot be zero.");
    AssertValidTextureFormatType(format);

    UInt8 numChannels = textureFormatType::GetChannelCount(format);
    UInt8 numBytes = textureFormatType::GetBytesPerChannel(format);
    SizeT expectedBufferSize = static_cast<SizeT>(width)
      * static_cast<SizeT>(height)
      * static_cast<SizeT>(numChannels)
      * static_cast<SizeT>(numBytes);

    if (m_data.size() != expectedBufferSize)
      throw InvalidArgumentException(
        String::Format("Buffer size (%zu) does not match expected size (%zu) for image dimensions (%u x %u) and format type %d.",
          m_data.size(),
          expectedBufferSize,
          width, height,
          static_cast<UInt8>(format)
        )
      );
  }

  Image::Image(
    UInt32 width,
    UInt32 height,
    textureFormatType::Type format,
    colorSpaceType::Type colorSpace,
    const Color& initColor
  ) :
    Asset(Path()),
    m_width(width),
    m_height(height),
    m_format(format),
    m_colorSpace(colorSpace),
    m_data()
  {
    if (width == 0)
      throw InvalidArgumentException("Image width cannot be zero.");
    if (height == 0)
      throw InvalidArgumentException("Image height cannot be zero.");
    AssertValidTextureFormatType(format);

    UInt8 numChannels = textureFormatType::GetChannelCount(format);
    UInt8 numBytes = textureFormatType::GetBytesPerChannel(format);
    UInt32 numPixels = width * height;
    UInt32 pixelSize = static_cast<UInt32>(numChannels) * static_cast<UInt32>(numBytes);

    m_data.reset(static_cast<SizeT>(numPixels) * static_cast<SizeT>(pixelSize));

    if (format == textureFormatType::RGB8 || format == textureFormatType::RGBA8)
    {
      for (UInt32 i = 0; i < numPixels; ++i)
      {
        UInt32 pixelOffset = i * pixelSize;
        m_data[pixelOffset + 0] = static_cast<Byte>(initColor.r * 255.0f);
        if (numChannels > 1)
          m_data[pixelOffset + 1] = static_cast<Byte>(initColor.g * 255.0f);
        if (numChannels > 2)
          m_data[pixelOffset + 2] = static_cast<Byte>(initColor.b * 255.0f);
        if (numChannels > 3)
          m_data[pixelOffset + 3] = static_cast<Byte>(initColor.a * 255.0f);
      }
    }
    else if (format == textureFormatType::Depth16)
    {
      for (UInt32 i = 0; i < numPixels; ++i)
      {
        UInt32 pixelOffset = i * pixelSize;
        UInt16* pixelData = reinterpret_cast<UInt16*>(&m_data[pixelOffset]);
        pixelData[0] = static_cast<UInt16>(initColor.r * 65535.0f);
      }
    }
    else if (
      format == textureFormatType::RGB32F
      || format == textureFormatType::RGBA32F
      || format == textureFormatType::Depth32F)
    {
      for (UInt32 i = 0; i < numPixels; ++i)
      {
        UInt32 pixelOffset = i * pixelSize;
        float* pixelData = reinterpret_cast<float*>(&m_data[pixelOffset]);
        pixelData[0] = initColor.r;
        if (numChannels > 1)
          pixelData[1] = initColor.g;
        if (numChannels > 2)
          pixelData[2] = initColor.b;
        if (numChannels > 3)
          pixelData[3] = initColor.a;
      }
    }
    else
    {
      throw InvalidArgumentException(
        String::Format("Unsupported texture format type %s for image initialization.",
          textureFormatType::ToString(format).c_str()
        )
      );
    }
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

  void Image::flipVertically()
  {
    UInt8 numChannels = textureFormatType::GetChannelCount(m_format);
    UInt8 numBytesPerChannel = textureFormatType::GetBytesPerChannel(m_format);
    UInt32 rowSize = m_width
      * static_cast<UInt32>(numChannels)
      * static_cast<UInt32>(numBytesPerChannel);
    UInt32 halfHeight = static_cast<UInt32>(m_height / 2);

    BufferByte tempRow(rowSize);
    for (UInt32 y = 0; y < halfHeight; ++y)
    {
      UInt32 topRowOffset = y * rowSize;
      UInt32 bottomRowOffset = (m_height - 1 - y) * rowSize;
      std::memcpy(tempRow.data(), &m_data[topRowOffset], rowSize);
      std::memcpy(&m_data[topRowOffset], &m_data[bottomRowOffset], rowSize);
      std::memcpy(&m_data[bottomRowOffset], tempRow.data(), rowSize);
    }
  }
}
