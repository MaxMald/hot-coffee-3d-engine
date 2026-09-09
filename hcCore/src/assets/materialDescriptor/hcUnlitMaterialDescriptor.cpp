#include "hc/assets/materialDescriptor/hcUnlitMaterialDescriptor.h"

namespace hc
{
  static constexpr UInt16 UNLIT_MATERIAL_DESCRIPTOR_VERSION = 1;

  UnlitMaterialDescriptor::UnlitMaterialDescriptor() :
    AMaterialDescriptor(""),
    color(0.5f, 0.5f, 0.5f, 1.0f),
    mainImagePath("")
  {}

  UnlitMaterialDescriptor::UnlitMaterialDescriptor(const Path& path) :
    AMaterialDescriptor(path),
    color(0.5f, 0.5f, 0.5f, 1.0f),
    mainImagePath("")
  {}

  materialType::Type UnlitMaterialDescriptor::getType() const
  {
    return materialType::Unlit;
  }

  void UnlitMaterialDescriptor::getImagesPaths(Vector<Path>& paths) const
  {
    if (!mainImagePath.empty())
      paths.push_back(mainImagePath);
  }

  void UnlitMaterialDescriptor::clear()
  {
    AMaterialDescriptor::clear();
    color = Color(0.5f, 0.5f, 0.5f, 1.0f);
    mainImagePath.clear();
  }

  void UnlitMaterialDescriptor::onSerialization(io::BinaryWriter& writer) const
  {
    writer.writeColor(color);
    writer.writePath(mainImagePath);
  }

  void UnlitMaterialDescriptor::onDeserialization(io::BinaryReader& reader)
  {
    color = reader.readColor();
    mainImagePath = reader.readPath();
  }

  UInt16 UnlitMaterialDescriptor::getDerivedVersion() const
  {
    return UNLIT_MATERIAL_DESCRIPTOR_VERSION;
  }
}
