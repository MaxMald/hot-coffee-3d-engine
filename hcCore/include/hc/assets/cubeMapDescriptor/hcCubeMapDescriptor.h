#pragma once

#include "hc/assets/hcAsset.h"

namespace hc
{
  struct CubeMapDescriptor : public Asset, public ISerializable
  {
  public:
    UInt32 m_faceWidth;
    UInt32 m_faceHeight;
    UInt8 m_channels;
    Path m_rightImagePath;
    Path m_leftImagePath;
    Path m_topImagePath;
    Path m_bottomImagePath;
    Path m_backImagePath;
    Path m_frontImagePath;

    CubeMapDescriptor();
    CubeMapDescriptor(const Path& path);
    ~CubeMapDescriptor() override = default;

    void serialize(BinaryWriter& writer) const override;
    void deserialize(BinaryReader& reader) override;
  };
}
