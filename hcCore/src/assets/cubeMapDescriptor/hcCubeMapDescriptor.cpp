#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptor.h"

namespace hc
{
  CubeMapDescriptor::CubeMapDescriptor() :
    Asset(""),
    m_faceWidth(0), m_faceHeight(0), m_channels(4),
    m_rightImagePath(), m_leftImagePath(), m_topImagePath(),
    m_bottomImagePath(), m_backImagePath(), m_frontImagePath()
  {}

  CubeMapDescriptor::CubeMapDescriptor(const Path& path) :
    Asset(path),
    m_faceWidth(0), m_faceHeight(0), m_channels(4),
    m_rightImagePath(), m_leftImagePath(), m_topImagePath(),
    m_bottomImagePath(), m_backImagePath(), m_frontImagePath()
  {}

  void CubeMapDescriptor::serialize(BinaryWriter & writer) const
  {
    writer.writePath(m_path);
    writer.writeUInt32(m_faceWidth);
    writer.writeUInt32(m_faceHeight);
    writer.writeUInt8(m_channels);
    writer.writePath(m_rightImagePath);
    writer.writePath(m_leftImagePath);
    writer.writePath(m_topImagePath);
    writer.writePath(m_bottomImagePath);
    writer.writePath(m_backImagePath);
    writer.writePath(m_frontImagePath);
  }

  void CubeMapDescriptor::deserialize(BinaryReader & reader)
  {
    m_path.clear();
    m_faceWidth = 0;
    m_faceHeight = 0;
    m_channels = 4;
    m_rightImagePath.clear();
    m_leftImagePath.clear();
    m_topImagePath.clear();
    m_bottomImagePath.clear();
    m_backImagePath.clear();
    m_frontImagePath.clear();

    m_path = reader.readPath();
    m_faceWidth = reader.readUInt32();
    m_faceHeight = reader.readUInt32();
    m_channels = reader.readUInt8();
    m_rightImagePath = reader.readPath();
    m_leftImagePath = reader.readPath();
    m_topImagePath = reader.readPath();
    m_bottomImagePath = reader.readPath();
    m_backImagePath = reader.readPath();
    m_frontImagePath = reader.readPath();
  }
}
