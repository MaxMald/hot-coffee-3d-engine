#pragma once

#include "hc/assets/hcAsset.h"
#include "hc/utilities/hcTextureFormatType.h"

namespace hc
{
  struct HC_CORE_EXPORT CubeMapDescriptor : public Asset, public io::ISerializable
  {
    UInt32 faceSize;                ///< Size of each face of the cube map texture (width and height)
    textureFormatType::Type format; ///< Format of the cube map texture
    Path rightImagePath;            ///< Path to the right face image
    Path leftImagePath;             ///< Path to the left face image
    Path topImagePath;              ///< Path to the top face image
    Path bottomImagePath;           ///< Path to the bottom face image
    Path backImagePath;             ///< Path to the back face image
    Path frontImagePath;            ///< Path to the front face image

    CubeMapDescriptor();
    CubeMapDescriptor(const Path& path);
    ~CubeMapDescriptor() override = default;

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;

    void clear();
  };
}
