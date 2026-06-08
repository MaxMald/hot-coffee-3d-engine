#pragma once

#include "hc/assets/hcAsset.h"

namespace hc
{
  struct HC_CORE_EXPORT CubeMapDescriptor : public Asset, public ISerializable
  {
    UInt32 faceWidth;
    UInt32 faceHeight;
    UInt8 channels;
    bool hasRelativePaths;
    Path rightImagePath;
    Path leftImagePath;
    Path topImagePath;
    Path bottomImagePath;
    Path backImagePath;
    Path frontImagePath;

    CubeMapDescriptor();
    CubeMapDescriptor(const Path& path);
    ~CubeMapDescriptor() override = default;

    void serialize(BinaryWriter& writer) const override;
    void deserialize(BinaryReader& reader) override;

    void clear();
  };
}
