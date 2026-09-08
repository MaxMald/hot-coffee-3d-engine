#include "hc/assets/materialDescriptor/hcUnlitMaterialDescriptor.h"

namespace hc
{
  static constexpr UInt16 UNLIT_MATERIAL_DESCRIPTOR_VERSION = 1;

  UnlitMaterialDescriptor::UnlitMaterialDescriptor() :
    AMaterialDescriptor(""),
    m_color(0.5f, 0.5f, 0.5f, 1.0f),
    m_mainImagePath("")
  {}

  UnlitMaterialDescriptor::UnlitMaterialDescriptor(
    const Path& path,
    const String& name,
    const Color& color,
    const Path& mainImagePath
  ) :
    AMaterialDescriptor(path, name),
    m_color(color),
    m_mainImagePath(mainImagePath)
  {}

  materialType::Type UnlitMaterialDescriptor::getType() const
  {
    return materialType::Unlit;
  }

  void UnlitMaterialDescriptor::getImagesPaths(Vector<Path>& paths) const
  {
    if (!m_mainImagePath.empty())
      paths.push_back(m_mainImagePath);
  }

  void UnlitMaterialDescriptor::clear()
  {
    AMaterialDescriptor::clear();
    m_color = Color(0.5f, 0.5f, 0.5f, 1.0f);
    m_mainImagePath.clear();
  }

  const Color& UnlitMaterialDescriptor::getColor() const
  {
    return m_color;
  }

  const Path& UnlitMaterialDescriptor::getMainImagePath() const
  {
    return m_mainImagePath;
  }

  void UnlitMaterialDescriptor::onSerialization(io::BinaryWriter& writer) const
  {
    writer.writeColor(m_color);
    writer.writePath(m_mainImagePath);
  }

  void UnlitMaterialDescriptor::onDeserialization(io::BinaryReader& reader)
  {
    m_color = reader.readColor();
    m_mainImagePath = reader.readPath();
  }

  UInt16 UnlitMaterialDescriptor::getDerivedVersion() const
  {
    return UNLIT_MATERIAL_DESCRIPTOR_VERSION;
  }
}
