#include "hc/assets/materialDescriptor/hcHairMaterialDescriptor.h"

namespace hc
{
  static constexpr UInt16 HAIR_MATERIAL_DESCRIPTOR_VERSION = 1;

  HairMaterialDescriptor::HairMaterialDescriptor(
    const Path& path,
    const String& name,
    const Color& color,
    float shininess,
    const Path& albedoImagePath,
    const Path& normalImagePath,
    const Path& specularImagePath
  ) :
    AMaterialDescriptor(path, name),
    m_color(color),
    m_shininess(Math::Clamp(shininess, 1.0f, 256.0f)),
    m_albedoImagePath(albedoImagePath),
    m_normalImagePath(normalImagePath),
    m_specularImagePath(specularImagePath)
  {
  }

  materialType::Type HairMaterialDescriptor::getType() const
  {
    return materialType::Hair;
  }

  void HairMaterialDescriptor::getImagesPaths(Vector<Path>& paths) const
  {
    if (!m_albedoImagePath.empty())
      paths.push_back(m_albedoImagePath);
    if (!m_normalImagePath.empty())
      paths.push_back(m_normalImagePath);
    if (!m_specularImagePath.empty())
      paths.push_back(m_specularImagePath);
  }

  void HairMaterialDescriptor::clear()
  {
    AMaterialDescriptor::clear();
    m_color = Color::White();
    m_shininess = 32.0f;
    m_albedoImagePath.clear();
    m_normalImagePath.clear();
    m_specularImagePath.clear();
  }

  const Color& HairMaterialDescriptor::getColor() const
  {
    return m_color;
  }

  float HairMaterialDescriptor::getShininess() const
  {
    return m_shininess;
  }

  const Path& HairMaterialDescriptor::getAlbedoImagePath() const
  {
    return m_albedoImagePath;
  }

  const Path& HairMaterialDescriptor::getNormalImagePath() const
  {
    return m_normalImagePath;
  }

  const Path& HairMaterialDescriptor::getSpecularImagePath() const
  {
    return m_specularImagePath;
  }

  void HairMaterialDescriptor::onSerialization(io::BinaryWriter& writer) const
  {
    writer.writeColor(m_color);
    writer.writeFloat(m_shininess);
    writer.writePath(m_albedoImagePath);
    writer.writePath(m_normalImagePath);
    writer.writePath(m_specularImagePath);
  }

  void HairMaterialDescriptor::onDeserialization(io::BinaryReader& reader)
  {
    m_color = reader.readColor();
    m_shininess = Math::Clamp(reader.readFloat(), 1.0f, 256.0f);
    m_albedoImagePath = reader.readPath();
    m_normalImagePath = reader.readPath();
    m_specularImagePath = reader.readPath();
  }

  UInt16 HairMaterialDescriptor::getDerivedVersion() const
  {
    return HAIR_MATERIAL_DESCRIPTOR_VERSION;
  }
}
