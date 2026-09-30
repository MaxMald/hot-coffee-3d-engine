#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace textureFormatType
  {
    /**
     * @brief Enumeration for different texture format types. These types represent the
     * format in which the texture or image data is stored.
     */
    enum Type : UInt8
    {
      // Color formats
      RGB8,
      RGBA8,
      RGB16F,
      RGBA16F,
      RGB32F,
      RGBA32F,

      // Depth formats
      Depth16,
      Depth24,
      Depth32F,

      Count,
      Undefined
    };

    /**
     * @brief Gets the number of channels for the specified texture format.
     * @param format The texture format type.
     * @return The number of channels (1 for depth formats, 3 for RGB, 4 for RGBA).
     * @throws RuntimeErrorException if the format is not implemented.
     */
    inline UInt8 GetChannelCount(Type format)
    {
      switch (format)
      {
      case RGBA8:
      case RGBA16F:
      case RGBA32F:
        return 4;
      case RGB8:
      case RGB16F:
      case RGB32F:
        return 3;
      case Depth16:
      case Depth24:
      case Depth32F:
        return 1;
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: getChannelCount for texture format type %d", static_cast<UInt8>(format)));
      }
    }

    /**
     * @brief Gets the number of bits per channel for the specified texture format.
     * @param format The texture format type.
     * @return The number of bits per channel (8, 16, 24, or 32).
     * @throws RuntimeErrorException if the format is not implemented.
     */
    inline UInt8 GetBitsPerChannel(Type format)
    {
      switch (format)
      {
      case RGBA8:
      case RGB8:
        return 8;
      case RGBA16F:
      case RGB16F:
      case Depth16:
        return 16;
      case Depth24:
        return 24;
      case RGBA32F:
      case RGB32F:
      case Depth32F:
        return 32;
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: getBitsPerChannel for texture format type %d", static_cast<UInt8>(format)));
      }
    }

    /**
     * @brief Gets the number of bytes per channel for the specified texture format.
     * @param format The texture format type.
     * @return The number of bytes per channel (1, 2, 3, or 4).
     * @throws RuntimeErrorException if the format is not implemented.
     */
    inline UInt8 GetBytesPerChannel(Type format)
    {
      switch (format)
      {
      case RGBA8:
      case RGB8:
        return 1;
      case RGBA16F:
      case RGB16F:
      case Depth16:
        return 2;
      case Depth24:
        return 3;
      case Depth32F:
      case RGBA32F:
      case RGB32F:
        return 4;
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: getBytesPerChannel for texture format type %d", static_cast<UInt8>(format)));
      }
    }

    inline String ToString(Type format)
    {
      switch (format)
      {
      case RGBA8:
        return "RGBA8";
      case RGB8:
        return "RGB8";
      case RGBA16F:
        return "RGBA16F";
      case RGB16F:
        return "RGB16F";
      case Depth16:
        return "Depth16";
      case RGB32F:
        return "RGB32F";
      case RGBA32F:
        return "RGBA32F";
      case Depth24:
        return "Depth24";
      case Depth32F:
        return "Depth32F";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: toString for texture format type %d", static_cast<UInt8>(format)));
      }
    }

    inline Type FromString(const String& str)
    {
      if (str == "RGBA8")
        return RGBA8;
      if (str == "RGB8")
        return RGB8;
      if (str == "RGB16F")
        return RGB16F;
      if (str == "RGBA16F")
        return RGBA16F;
      if (str == "Depth16")
        return Depth16;
      if (str == "RGB32F")
        return RGB32F;
      if (str == "RGBA32F")
        return RGBA32F;
      if (str == "Depth24")
        return Depth24;
      if (str == "Depth32F")
        return Depth32F;
      throw RuntimeErrorException(
        String::Format("Not implemented: fromString for texture format type string '%s'", str.c_str()));
    }
  }

  namespace colorSpaceType
  {
    /**
     * @brief Enumeration for different color space types. These types represent the
     * color space in which the texture or image data is stored.
     */
    enum Type : UInt8
    {
      SRGB,
      Linear,
      Count
    };

    inline String ToString(Type colorSpace)
    {
      switch (colorSpace)
      {
      case SRGB:
        return "SRGB";
      case Linear:
        return "Linear";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented: toString for color space type %d", static_cast<UInt8>(colorSpace)));
      }
    }

    inline Type FromString(const String& colorSpaceStr)
    {
      if (colorSpaceStr == "SRGB")
        return SRGB;
      else if (colorSpaceStr == "Linear")
        return Linear;
      else
        throw RuntimeErrorException(
          String::Format("Not implemented: getColorSpaceTypeFromString for color space type string '%s'", colorSpaceStr.c_str()));
    }
  }

  namespace platformType
  {
    /**
     * @brief Enumeration for different platform types. These types represent the
     * operating system or platform on which the application is running.
     */
    enum Type : UInt8
    {
      UNKNOWN = 0,
      WINDOWS
    };
  }
}
