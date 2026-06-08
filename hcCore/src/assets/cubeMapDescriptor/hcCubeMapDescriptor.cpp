#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"

namespace hc
{
  CubeMapDescriptor::CubeMapDescriptor() :
    Asset(""),
    faceWidth(0), faceHeight(0), channels(4),
    rightImagePath(), leftImagePath(), topImagePath(),
    bottomImagePath(), backImagePath(), frontImagePath()
  {}

  CubeMapDescriptor::CubeMapDescriptor(const Path& path) :
    Asset(path),
    faceWidth(0), faceHeight(0), channels(4),
    rightImagePath(), leftImagePath(), topImagePath(),
    bottomImagePath(), backImagePath(), frontImagePath()
  {}

  void CubeMapDescriptor::serialize(BinaryWriter & writer) const
  {
    writer.writeUInt32(faceWidth);
    writer.writeUInt32(faceHeight);
    writer.writeUInt8(channels);
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
    faceWidth = reader.readUInt32();
    faceHeight = reader.readUInt32();
    channels = reader.readUInt8();
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
    faceWidth = 0;
    faceHeight = 0;
    channels = 4;
    rightImagePath.clear();
    leftImagePath.clear();
    topImagePath.clear();
    bottomImagePath.clear();
    backImagePath.clear();
    frontImagePath.clear();
  }
}
