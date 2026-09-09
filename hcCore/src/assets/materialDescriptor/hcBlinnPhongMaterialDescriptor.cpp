#include "hc/assets/materialDescriptor/hcBlinnPhongMaterialDescriptor.h"

namespace hc
{
  static constexpr UInt16 BLINN_PHONG_MATERIAL_DESCRIPTOR_VERSION = 1;

  BlinnPhongMaterialDescriptor::BlinnPhongMaterialDescriptor() :
    AMaterialDescriptor(""),
    color(Color::White()),
    shininess(32.0f),
    albedoImagePath(""),
    normalImagePath(""),
    specularImagePath("")
  {
  }

  BlinnPhongMaterialDescriptor::BlinnPhongMaterialDescriptor(const Path& path) :
    AMaterialDescriptor(path),
    color(Color::White()),
    shininess(32.0f),
    albedoImagePath(""),
    normalImagePath(""),
    specularImagePath("")
  {}

  materialType::Type BlinnPhongMaterialDescriptor::getType() const
  {
    return materialType::BlinnPhong;
  }

  void BlinnPhongMaterialDescriptor::getImagesPaths(Vector<Path>& paths) const
  {
    if (!albedoImagePath.empty())
      paths.push_back(albedoImagePath);
    if (!normalImagePath.empty())
      paths.push_back(normalImagePath);
    if (!specularImagePath.empty())
      paths.push_back(specularImagePath);
  }

  void BlinnPhongMaterialDescriptor::clear()
  {
    AMaterialDescriptor::clear();
    color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    shininess = 32.0f;
    albedoImagePath.clear();
    normalImagePath.clear();
    specularImagePath.clear();
  }

  void BlinnPhongMaterialDescriptor::onSerialization(io::BinaryWriter& writer) const
  {
    writer.writeColor(color);
    writer.writeFloat(shininess);
    writer.writePath(albedoImagePath);
    writer.writePath(normalImagePath);
    writer.writePath(specularImagePath);
  }

  void BlinnPhongMaterialDescriptor::onDeserialization(io::BinaryReader& reader)
  {
    color = reader.readColor();
    shininess = reader.readFloat();
    albedoImagePath = reader.readPath();
    normalImagePath = reader.readPath();
    specularImagePath = reader.readPath();
  }

  UInt16 BlinnPhongMaterialDescriptor::getDerivedVersion() const
  {
    return BLINN_PHONG_MATERIAL_DESCRIPTOR_VERSION;
  }
}
