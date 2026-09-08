#include "hc/assets/materialDescriptor/hcBlinnPhongMaterialDescriptor.h"

namespace hc
{
  static constexpr UInt16 BLINN_PHONG_MATERIAL_DESCRIPTOR_VERSION = 1;

  BlinnPhongMaterialDescriptor::BlinnPhongMaterialDescriptor(
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
  {}

  materialType::Type BlinnPhongMaterialDescriptor::getType() const
  {
    return materialType::BlinnPhong;
  }

  void BlinnPhongMaterialDescriptor::getImagesPaths(Vector<Path>& paths) const
  {
    if (!m_albedoImagePath.empty())
      paths.push_back(m_albedoImagePath);
    if (!m_normalImagePath.empty())
      paths.push_back(m_normalImagePath);
    if (!m_specularImagePath.empty())
      paths.push_back(m_specularImagePath);
  }

  void BlinnPhongMaterialDescriptor::clear()
  {
    AMaterialDescriptor::clear();
    m_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    m_shininess = 32.0f;
    m_albedoImagePath.clear();
    m_normalImagePath.clear();
    m_specularImagePath.clear();
  }

  const Color& BlinnPhongMaterialDescriptor::getColor() const
  {
    return m_color;
  }

  float BlinnPhongMaterialDescriptor::getShininess() const
  {
    return m_shininess;
  }

  const Path& BlinnPhongMaterialDescriptor::getAlbedoImagePath() const
  {
    return m_albedoImagePath;
  }

  const Path& BlinnPhongMaterialDescriptor::getNormalImagePath() const
  {
    return m_normalImagePath;
  }

  const Path& BlinnPhongMaterialDescriptor::getSpecularImagePath() const
  {
    return m_specularImagePath;
  }

  void BlinnPhongMaterialDescriptor::onSerialization(io::BinaryWriter& writer) const
  {
    writer.writeColor(m_color);
    writer.writeFloat(m_shininess);
    writer.writePath(m_albedoImagePath);
    writer.writePath(m_normalImagePath);
    writer.writePath(m_specularImagePath);
  }

  void BlinnPhongMaterialDescriptor::onDeserialization(io::BinaryReader& reader)
  {
    m_color = reader.readColor();
    m_shininess = reader.readFloat();
    m_albedoImagePath = reader.readPath();
    m_normalImagePath = reader.readPath();
    m_specularImagePath = reader.readPath();
  }

  UInt16 BlinnPhongMaterialDescriptor::getDerivedVersion() const
  {
    return BLINN_PHONG_MATERIAL_DESCRIPTOR_VERSION;
  }
}
