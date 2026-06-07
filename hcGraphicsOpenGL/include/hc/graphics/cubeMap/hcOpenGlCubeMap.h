#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlCubeMap : public ICubeMap
  {
  public:
    OpenGlCubeMap();
    ~OpenGlCubeMap() override;

    void initialize(
      const UInt32 width,
      const UInt32 height,
      const UInt8 channels,
      const Image& right,
      const Image& left,
      const Image& top,
      const Image& bottom,
      const Image& back,
      const Image& front
    ) override;

    bool isValid() const override;
    UInt32 getFaceWidth() const override;
    UInt32 getFaceHeight() const override;
    UInt8 getChannels() const override;
    void destroy() override;

    UInt32 getId() const;

  private:
    UInt32 m_id;
    bool m_valid;
    UInt32 m_faceWidth;
    UInt32 m_faceHeight;
    UInt8 m_channels;

    static void assertImageSize(const Image& image, const UInt32 width, const UInt32 height);
    void assertIsValid() const;
  };
}
