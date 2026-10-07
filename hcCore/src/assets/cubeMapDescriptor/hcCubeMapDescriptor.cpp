#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"

namespace hc
{
  static constexpr UInt32 CUBEMAP_DESCRIPTOR_VERSION = 1;

  CubeMapDescriptor::CubeMapDescriptor() :
    Asset(""),
    faceSize(0), format(textureFormatType::RGBA8),
    pXImagePath(), nXImagePath(), pYImagePath(),
    nYImagePath(), pZImagePath(), nZImagePath()
  {}

  CubeMapDescriptor::CubeMapDescriptor(const Path& path) :
    Asset(path),
    faceSize(0), format(textureFormatType::RGBA8),
    pXImagePath(), nXImagePath(), pYImagePath(),
    nYImagePath(), pZImagePath(), nZImagePath()
  {}

  void CubeMapDescriptor::serialize(io::BinaryWriter & writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(0), CUBEMAP_DESCRIPTOR_VERSION);
    writer.writeUInt32(faceSize);
    writer.writeUInt8(static_cast<UInt8>(format));
    writer.writePath(pXImagePath);
    writer.writePath(nXImagePath);
    writer.writePath(pYImagePath);
    writer.writePath(nYImagePath);
    writer.writePath(pZImagePath);
    writer.writePath(nZImagePath);
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
    pXImagePath = reader.readPath();
    nXImagePath = reader.readPath();
    pYImagePath = reader.readPath();
    nYImagePath = reader.readPath();
    pZImagePath = reader.readPath();
    nZImagePath = reader.readPath();
    reader.finishReadingObject();
  }

  void CubeMapDescriptor::clear()
  {
    path.clear();
    faceSize = 0;
    format = textureFormatType::RGBA8;
    pXImagePath.clear();
    nXImagePath.clear();
    pYImagePath.clear();
    nYImagePath.clear();
    pZImagePath.clear();
    nZImagePath.clear();
  }
}
