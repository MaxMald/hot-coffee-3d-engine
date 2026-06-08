#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"

namespace hc
{
  CubeMapDescriptor::CubeMapDescriptor() :
    Asset(""),
    faceWidth(0), faceHeight(0), channels(4), hasRelativePaths(false),
    rightImagePath(), leftImagePath(), topImagePath(),
    bottomImagePath(), backImagePath(), frontImagePath()
  {}

  CubeMapDescriptor::CubeMapDescriptor(const Path& path) :
    Asset(path),
    faceWidth(0), faceHeight(0), channels(4), hasRelativePaths(false),
    rightImagePath(), leftImagePath(), topImagePath(),
    bottomImagePath(), backImagePath(), frontImagePath()
  {}

  void CubeMapDescriptor::serialize(BinaryWriter & writer) const
  {
    writer.writePath(m_path);
    writer.writeUInt32(faceWidth);
    writer.writeUInt32(faceHeight);
    writer.writeUInt8(channels);
    writer.writeBool(hasRelativePaths);
    writer.writePath(rightImagePath);
    writer.writePath(leftImagePath);
    writer.writePath(topImagePath);
    writer.writePath(bottomImagePath);
    writer.writePath(backImagePath);
    writer.writePath(frontImagePath);
  }

  void CubeMapDescriptor::deserialize(BinaryReader & reader)
  {
    clear();
    m_path = reader.readPath();
    faceWidth = reader.readUInt32();
    faceHeight = reader.readUInt32();
    channels = reader.readUInt8();
    hasRelativePaths = reader.readBool();
    rightImagePath = reader.readPath();
    leftImagePath = reader.readPath();
    topImagePath = reader.readPath();
    bottomImagePath = reader.readPath();
    backImagePath = reader.readPath();
    frontImagePath = reader.readPath();
  }

  void CubeMapDescriptor::clear()
  {
    m_path.clear();
    faceWidth = 0;
    faceHeight = 0;
    channels = 4;
    hasRelativePaths = false;
    rightImagePath.clear();
    leftImagePath.clear();
    topImagePath.clear();
    bottomImagePath.clear();
    backImagePath.clear();
    frontImagePath.clear();
  }
}
