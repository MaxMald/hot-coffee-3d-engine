#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"

namespace hc
{
  static constexpr UInt32 CUBEMAP_DESCRIPTOR_VERSION = 1;

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

  void CubeMapDescriptor::serialize(io::BinaryWriter & writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(0), CUBEMAP_DESCRIPTOR_VERSION);
    writer.writeUInt32(faceSize);
    writer.writeUInt8(static_cast<UInt8>(format));
    writer.writePath(rightImagePath);
    writer.writePath(leftImagePath);
    writer.writePath(topImagePath);
    writer.writePath(bottomImagePath);
    writer.writePath(backImagePath);
    writer.writePath(frontImagePath);
    writer.finishWritingObject();
  }

  void CubeMapDescriptor::deserialize(io::BinaryReader & reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchVersion(CUBEMAP_DESCRIPTOR_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    faceSize = reader.readUInt32();
    format = static_cast<textureFormatType::Type>(reader.readUInt8());
    rightImagePath = reader.readPath();
    leftImagePath = reader.readPath();
    topImagePath = reader.readPath();
    bottomImagePath = reader.readPath();
    backImagePath = reader.readPath();
    frontImagePath = reader.readPath();
    reader.finishReadingObject();
  }

  void CubeMapDescriptor::clear()
  {
    path.clear();
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
