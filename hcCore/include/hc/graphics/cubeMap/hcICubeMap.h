#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Image;

  class HC_CORE_EXPORT ICubeMap : public NonCopyable
  {
  public:
    virtual ~ICubeMap();

    virtual void initialize(
      const UInt32 width,
      const UInt32 height,
      const UInt8 channels,
      const Image& right,
      const Image& left,
      const Image& top,
      const Image& bottom,
      const Image& back,
      const Image& front
    ) = 0;

    virtual bool isValid() const = 0;
    virtual UInt32 getFaceWidth() const = 0;
    virtual UInt32 getFaceHeight() const = 0;
    virtual UInt8 getChannels() const = 0;
    virtual void destroy() = 0;

  protected:
    ICubeMap();
  };
}
