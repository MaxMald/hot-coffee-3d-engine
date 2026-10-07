#pragma once

#include "hc/hcCoreCommons.h"
#include "hc/assets/hcAsset.h"

namespace hc
{
  struct HC_CORE_EXPORT CubeMapDescriptor : public Asset, public io::ISerializable
  {
    UInt32 faceSize;                ///< Size of each face of the cube map texture (width and height)
    textureFormatType::Type format; ///< Format of the cube map texture
    Path pXImagePath;               ///< Path to the positive X face image
    Path nXImagePath;               ///< Path to the negative X face image
    Path pYImagePath;               ///< Path to the positive Y face image
    Path nYImagePath;               ///< Path to the negative Y face image
    Path pZImagePath;               ///< Path to the positive Z face image
    Path nZImagePath;               ///< Path to the negative Z face image

    CubeMapDescriptor();
    CubeMapDescriptor(const Path& path);
    ~CubeMapDescriptor() override = default;

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;
    void clear();
  };
}
