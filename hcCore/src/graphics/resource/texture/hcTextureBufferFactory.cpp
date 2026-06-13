#include "hc/graphics/resource/texture/hcTextureBufferFactory.h"

namespace hc
{
  void TextureBufferFactory::CreateRGB8(
    UInt32 width,
    UInt32 height,
    const Color& initColor,
    BufferByte& outBuffer
  )
  {
    SizeT pixelCount = static_cast<SizeT>(width) * static_cast<SizeT>(height);
    outBuffer.reset(pixelCount * 3);
    for (SizeT i = 0; i < pixelCount; ++i)
    {
      outBuffer[i * 3 + 0] = static_cast<Byte>(initColor.r * 255.0f);
      outBuffer[i * 3 + 1] = static_cast<Byte>(initColor.g * 255.0f);
      outBuffer[i * 3 + 2] = static_cast<Byte>(initColor.b * 255.0f);
    }
  }

  void TextureBufferFactory::CreateRGBA8(
    UInt32 width,
    UInt32 height,
    const Color& initColor,
    BufferByte& outBuffer
  )
  {
    SizeT pixelCount = static_cast<SizeT>(width) * static_cast<SizeT>(height);
    outBuffer.reset(pixelCount * 4);
    for (SizeT i = 0; i < pixelCount; ++i)
    {
      outBuffer[i * 4 + 0] = static_cast<Byte>(initColor.r * 255.0f);
      outBuffer[i * 4 + 1] = static_cast<Byte>(initColor.g * 255.0f);
      outBuffer[i * 4 + 2] = static_cast<Byte>(initColor.b * 255.0f);
      outBuffer[i * 4 + 3] = static_cast<Byte>(initColor.a * 255.0f);
    }
  }

  void TextureBufferFactory::CreateRGB16F(
    UInt32 width,
    UInt32 height,
    const Color& initColor,
    BufferFloat& outBuffer
  )
  {
    SizeT pixelCount = static_cast<SizeT>(width) * static_cast<SizeT>(height);
    outBuffer.reset(pixelCount * 3);
    for (SizeT i = 0; i < pixelCount; ++i)
    {
      outBuffer[i * 3 + 0] = initColor.r;
      outBuffer[i * 3 + 1] = initColor.g;
      outBuffer[i * 3 + 2] = initColor.b;
    }
  }

  void TextureBufferFactory::CreateRGBA16F(
    UInt32 width,
    UInt32 height,
    const Color& initColor,
    BufferFloat& outBuffer
  )
  {
    SizeT pixelCount = static_cast<SizeT>(width) * static_cast<SizeT>(height);
    outBuffer.reset(pixelCount * 4);
    for (SizeT i = 0; i < pixelCount; ++i)
    {
      outBuffer[i * 4 + 0] = initColor.r;
      outBuffer[i * 4 + 1] = initColor.g;
      outBuffer[i * 4 + 2] = initColor.b;
      outBuffer[i * 4 + 3] = initColor.a;
    }
  }
}
