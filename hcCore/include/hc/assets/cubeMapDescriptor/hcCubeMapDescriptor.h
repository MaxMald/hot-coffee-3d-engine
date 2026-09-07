#pragma once

#include "hc/assets/hcAsset.h"
#include "hc/utilities/hcTextureFormatType.h"

namespace hc
{
  struct HC_CORE_EXPORT CubeMapDescriptor : public Asset, public io::ISerializable
  {
    UInt32 faceSize;
    textureFormatType::Type format;
    Path rightImagePath;
    Path leftImagePath;
    Path topImagePath;
    Path bottomImagePath;
    Path backImagePath;
    Path frontImagePath;

    CubeMapDescriptor();
    CubeMapDescriptor(const Path& path);
    ~CubeMapDescriptor() override = default;

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;

    void clear();
  };
}
