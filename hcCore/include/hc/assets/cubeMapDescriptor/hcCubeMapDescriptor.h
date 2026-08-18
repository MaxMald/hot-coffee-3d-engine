#pragma once

#include "hc/assets/hcAsset.h"
#include "hc/utilities/hcTextureFormatType.h"

namespace hc
{
  struct HC_CORE_EXPORT CubeMapDescriptor : public Asset, public ISerializable
  {
    UInt32 faceSize;
    textureFormatType::Type format;
    String rightImagePath;
    String leftImagePath;
    String topImagePath;
    String bottomImagePath;
    String backImagePath;
    String frontImagePath;

    CubeMapDescriptor();
    CubeMapDescriptor(const Path& path);
    ~CubeMapDescriptor() override = default;

    void serialize(BinaryWriter& writer) const override;
    void deserialize(BinaryReader& reader) override;

    void clear();
  };
}
