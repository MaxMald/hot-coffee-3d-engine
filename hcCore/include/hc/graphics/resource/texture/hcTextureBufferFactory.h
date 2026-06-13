#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  struct HC_CORE_EXPORT TextureBufferFactory
  {
    static void CreateRGB8(
      UInt32 width,
      UInt32 height,
      const Color& initColor,
      BufferByte& outBuffer
    );

    static void CreateRGBA8(
      UInt32 width,
      UInt32 height,
      const Color& initColor,
      BufferByte& outBuffer
    );

    static void CreateRGB16F(
      UInt32 width,
      UInt32 height,
      const Color& initColor,
      BufferFloat& outBuffer
    );

    static void CreateRGBA16F(
      UInt32 width,
      UInt32 height,
      const Color& initColor,
      BufferFloat& outBuffer
    );
  };
}
