#include "hc/assets/materialDescriptor/hcHairMaterialDescriptor.h"

namespace hc
{
  static constexpr UInt16 HAIR_MATERIAL_DESCRIPTOR_VERSION = 1;

  HairMaterialDescriptor::HairMaterialDescriptor() :
    AMaterialDescriptor(""),
    color(Color::White()),
    specularPrimaryColor(Color::White()),
    specularSecondaryColor(Color::White()),
    shininess(32.0f),
    specularPrimaryShift(0.0f),
    specularSecondaryShift(0.0f),
    specularWidth(1.0f),
    specularStrength(1.0f),
    albedoImagePath(""),
    normalImagePath(""),
    specularImagePath("")
  {}

  HairMaterialDescriptor::HairMaterialDescriptor(const Path& path) :
    AMaterialDescriptor(path),
    color(Color::White()),
    specularPrimaryColor(Color::White()),
    specularSecondaryColor(Color::White()),
    shininess(32.0f),
    specularPrimaryShift(0.0f),
    specularSecondaryShift(0.0f),
    specularWidth(1.0f),
    specularStrength(1.0f),
    albedoImagePath(""),
    normalImagePath(""),
    specularImagePath("")
  {}

  materialType::Type HairMaterialDescriptor::getType() const
  {
    return materialType::Hair;
  }

  void HairMaterialDescriptor::getImagesPaths(Vector<Path>& paths) const
  {
    if (!albedoImagePath.empty())
      paths.push_back(albedoImagePath);
    if (!normalImagePath.empty())
      paths.push_back(normalImagePath);
    if (!specularImagePath.empty())
      paths.push_back(specularImagePath);
  }

  void HairMaterialDescriptor::clear()
  {
    AMaterialDescriptor::clear();
    color = Color::White();
    specularPrimaryColor = Color::White();
    specularSecondaryColor = Color::White();
    shininess = 32.0f;
    specularPrimaryShift = 0.0f;
    specularSecondaryShift = 0.0f;
    specularWidth = 1.0f;
    specularStrength = 1.0f;
    albedoImagePath.clear();
    normalImagePath.clear();
    specularImagePath.clear();
  }

  void HairMaterialDescriptor::onSerialization(io::BinaryWriter& writer) const
  {
    writer.writeColor(color);
    writer.writeColor(specularPrimaryColor);
    writer.writeColor(specularSecondaryColor);
    writer.writeFloat(shininess);
    writer.writeFloat(specularPrimaryShift);
    writer.writeFloat(specularSecondaryShift);
    writer.writeFloat(specularWidth);
    writer.writeFloat(specularStrength);
    writer.writePath(albedoImagePath);
    writer.writePath(normalImagePath);
    writer.writePath(specularImagePath);
  }

  void HairMaterialDescriptor::onDeserialization(io::BinaryReader& reader)
  {
    color = reader.readColor();
    specularPrimaryColor = reader.readColor();
    specularSecondaryColor = reader.readColor();
    shininess = Math::Clamp(reader.readFloat(), 1.0f, 256.0f);
    specularPrimaryShift = reader.readFloat();
    specularSecondaryShift = reader.readFloat();
    specularWidth = reader.readFloat();
    specularStrength = reader.readFloat();
    albedoImagePath = reader.readPath();
    normalImagePath = reader.readPath();
    specularImagePath = reader.readPath();
  }

  UInt16 HairMaterialDescriptor::getDerivedVersion() const
  {
    return HAIR_MATERIAL_DESCRIPTOR_VERSION;
  }
}
