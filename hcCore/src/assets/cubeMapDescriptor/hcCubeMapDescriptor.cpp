#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"

namespace hc
{
  static constexpr UInt32 CUBEMAP_DESCRIPTOR_VERSION = 2;

  CubeMapDescriptor::CubeMapDescriptor() :
    Asset(""),
    pXImagePath(), nXImagePath(), pYImagePath(),
    nYImagePath(), pZImagePath(), nZImagePath(),
    format(textureFormatType::RGBA8)
  {}

  CubeMapDescriptor::CubeMapDescriptor(const Path& path) :
    Asset(path),
    pXImagePath(), nXImagePath(), pYImagePath(),
    nYImagePath(), pZImagePath(), nZImagePath(),
    format(textureFormatType::RGBA8)
  {}

  void CubeMapDescriptor::serialize(io::BinaryWriter & writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(0), CUBEMAP_DESCRIPTOR_VERSION);
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
    UInt32 version = header.version;
    if (version == 1)
    {
      reader.readUInt32(); // Read the face size but ignore it since it's not in further versions
    }
    if (version <= 2)
    {
      format = static_cast<textureFormatType::Type>(reader.readUInt8());
      pXImagePath = reader.readPath();
      nXImagePath = reader.readPath();
      pYImagePath = reader.readPath();
      nYImagePath = reader.readPath();
      pZImagePath = reader.readPath();
      nZImagePath = reader.readPath();
    }

    reader.finishReadingObject();
  }

  void CubeMapDescriptor::clear()
  {
    path.clear();
    format = textureFormatType::RGBA8;
    pXImagePath.clear();
    nXImagePath.clear();
    pYImagePath.clear();
    nYImagePath.clear();
    pZImagePath.clear();
    nZImagePath.clear();
  }
}
