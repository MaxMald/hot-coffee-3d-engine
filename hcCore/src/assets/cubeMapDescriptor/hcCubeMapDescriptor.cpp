#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"

namespace hc
{
  CubeMapDescriptor::CubeMapDescriptor() :
    Asset(""),
    faceSize(0), format(textureFormatType::RGBA8),
    rightImagePath(), leftImagePath(), topImagePath(),
    bottomImagePath(), backImagePath(), frontImagePath()
  {}

  CubeMapDescriptor::CubeMapDescriptor(const Path& path) :
    Asset(path),
    faceSize(0), format(textureFormatType::RGBA8),
    rightImagePath(), leftImagePath(), topImagePath(),
    bottomImagePath(), backImagePath(), frontImagePath()
  {}

  void CubeMapDescriptor::serialize(BinaryWriter & writer) const
  {
    writer.writeUInt32(faceSize);
    writer.writeUInt8(static_cast<UInt8>(format));
    writer.writeString(rightImagePath);
    writer.writeString(leftImagePath);
    writer.writeString(topImagePath);
    writer.writeString(bottomImagePath);
    writer.writeString(backImagePath);
    writer.writeString(frontImagePath);
  }

  void CubeMapDescriptor::deserialize(BinaryReader & reader)
  {
    clear();
    faceSize = reader.readUInt32();
    format = static_cast<textureFormatType::Type>(reader.readUInt8());
    rightImagePath = reader.readString();
    leftImagePath = reader.readString();
    topImagePath = reader.readString();
    bottomImagePath = reader.readString();
    backImagePath = reader.readString();
    frontImagePath = reader.readString();
  }

  void CubeMapDescriptor::clear()
  {
    m_path.clear();
    faceSize = 0;
    format = textureFormatType::RGBA8;
    rightImagePath.clear();
    leftImagePath.clear();
    topImagePath.clear();
    bottomImagePath.clear();
    backImagePath.clear();
    frontImagePath.clear();
  }
}
