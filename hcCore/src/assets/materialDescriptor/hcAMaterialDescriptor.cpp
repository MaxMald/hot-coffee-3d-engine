#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  namespace assets::materialDescriptor
  {
    // ------------ UNLIT DATA 

    static constexpr UInt32 UNLIT_DATA_VERSION = 1;

    void UnlitData::serialize(io::BinaryWriter& writer) const
    {
      writer.startWritingObject(materialType::Type::Unlit, UNLIT_DATA_VERSION);
      writer.writePath(textureImagePath);
      writer.writeColor(color);
      writer.finishWritingObject();
    }

    void UnlitData::deserialize(io::BinaryReader& reader)
    {
      clear();

      io::ObjectHeader header = reader.startReadingObject();
      if (!header.match(materialType::Type::Unlit, UNLIT_DATA_VERSION))
      {
        reader.finishReadingObject();
        return;
      }

      textureImagePath = reader.readPath();
      color = reader.readColor();
      reader.finishReadingObject();
    }

    void UnlitData::clear()
    {
      textureImagePath.clear();
      color = Color::White();
    }

    // ------------ BLINN PHONG DATA 

    static constexpr UInt32 BLINN_PHONG_DATA_VERSION = 1;

    void BlinnPhongData::serialize(io::BinaryWriter& writer) const
    {
      writer.startWritingObject(materialType::Type::BlinnPhong, BLINN_PHONG_DATA_VERSION);
      writer.writePath(diffuseImagePath);
      writer.writePath(normalImagePath);
      writer.writePath(specularImagePath);
      writer.writeColor(color);
      writer.writeFloat(shininess);
      writer.finishWritingObject();
    }

    void BlinnPhongData::deserialize(io::BinaryReader& reader)
    {
      clear();

      io::ObjectHeader header = reader.startReadingObject();
      if (!header.match(materialType::Type::BlinnPhong, BLINN_PHONG_DATA_VERSION))
      {
        reader.finishReadingObject();
        return;
      }

      diffuseImagePath = reader.readPath();
      normalImagePath = reader.readPath();
      specularImagePath = reader.readPath();
      color = reader.readColor();
      shininess = reader.readFloat();
      reader.finishReadingObject();
    }

    void BlinnPhongData::clear()
    {
      diffuseImagePath.clear();
      normalImagePath.clear();
      specularImagePath.clear();
      color = Color::White();
      shininess = 32.0f;
    }

    // ------------ HAIR DATA

    static constexpr UInt32 HAIR_DATA_VERSION = 1;

    void HairData::serialize(io::BinaryWriter& writer) const
    {
      writer.startWritingObject(materialType::Type::Hair, HAIR_DATA_VERSION);
      writer.writePath(albedoImagePath);
      writer.writePath(normalImagePath);
      writer.writePath(specularImagePath);
      writer.writeColor(color);
      writer.writeColor(specularPrimaryColor);
      writer.writeColor(specularSecondaryColor);
      writer.writeFloat(shininess);
      writer.writeFloat(specularPrimaryShift);
      writer.writeFloat(specularSecondaryShift);
      writer.writeFloat(specularWidth);
      writer.writeFloat(specularStrength);
      writer.finishWritingObject();
    }

    void HairData::deserialize(io::BinaryReader& reader)
    {
      clear();

      io::ObjectHeader header = reader.startReadingObject();
      if (!header.match(materialType::Type::Hair, HAIR_DATA_VERSION))
      {
        reader.finishReadingObject();
        return;
      }

      albedoImagePath = reader.readPath();
      normalImagePath = reader.readPath();
      specularImagePath = reader.readPath();
      color = reader.readColor();
      specularPrimaryColor = reader.readColor();
      specularSecondaryColor = reader.readColor();
      shininess = Math::Clamp(reader.readFloat(), 1.0f, 256.0f);
      specularPrimaryShift = reader.readFloat();
      specularSecondaryShift = reader.readFloat();
      specularWidth = reader.readFloat();
      specularStrength = reader.readFloat();
      reader.finishReadingObject();
    }

    void HairData::clear()
    {
      albedoImagePath.clear();
      normalImagePath.clear();
      specularImagePath.clear();
      color = Color::White();
      specularPrimaryColor = Color::White();
      specularSecondaryColor = Color::White();
      shininess = 32.0f;
      specularPrimaryShift = 0.0f;
      specularSecondaryShift = 0.0f;
      specularWidth = 1.0f;
      specularStrength = 1.0f;
    }
  }

  // ------------ MATERIAL DESCRIPTOR

  static constexpr UInt32 MATERIAL_DESCRIPTOR_VERSION = 1;

  MaterialDescriptor::MaterialDescriptor() :
    Asset(),
    variantData(assets::materialDescriptor::UnlitData()),
    name(""),
    alphaCutoutThreshold(0.5f),
    doubleSided(false),
    renderMode(materialRenderMode::Type::Opaque),
    type(materialType::Type::Unlit)
  {}

  MaterialDescriptor::MaterialDescriptor(const Path& path) :
    Asset(path),
    variantData(assets::materialDescriptor::UnlitData()),
    name(""),
    alphaCutoutThreshold(0.5f),
    doubleSided(false),
    renderMode(materialRenderMode::Type::Opaque),
    type(materialType::Type::Unlit)
  {}

  MaterialDescriptor::MaterialDescriptor(
    materialType::Type _type,
    const Path & _path
  ) :
    Asset(_path),
    variantData(assets::materialDescriptor::UnlitData()),
    name(""),
    alphaCutoutThreshold(0.5f),
    doubleSided(false),
    renderMode(materialRenderMode::Type::Opaque),
    type(_type)
  {
    setType(_type);
  }

  void MaterialDescriptor::setType(materialType::Type newType)
  {
    type = newType;
    switch (type)
    {
    case materialType::Type::Unlit:
      variantData = assets::materialDescriptor::UnlitData();
      break;
    case materialType::Type::BlinnPhong:
      variantData = assets::materialDescriptor::BlinnPhongData();
      break;
    case materialType::Type::Hair:
      variantData = assets::materialDescriptor::HairData();
      break;
    default:
      throw RuntimeErrorException("AMaterialDescriptor::setType: Unsupported material type.");
    }
  }

  void MaterialDescriptor::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(type), MATERIAL_DESCRIPTOR_VERSION);
    writer.writeUUID(m_uuid);
    writer.writePath(path);
    writer.writeString(name);
    writer.writeUInt8(static_cast<UInt8>(renderMode));
    writer.writeFloat(alphaCutoutThreshold);
    writer.writeBool(doubleSided);

    std::visit([&writer](const auto& variant) {
      variant.serialize(writer);
    }, variantData);

    writer.finishWritingObject();
  }

  void MaterialDescriptor::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchVersion(MATERIAL_DESCRIPTOR_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    type = static_cast<materialType::Type>(header.type);
    m_uuid = reader.readUUID();
    path = reader.readPath();
    name = reader.readString();
    renderMode = static_cast<materialRenderMode::Type>(reader.readUInt8());
    alphaCutoutThreshold = reader.readFloat();
    doubleSided = reader.readBool();

    switch (type)
    {
      case materialType::Type::Unlit:
      {
        assets::materialDescriptor::UnlitData unlit;
        unlit.deserialize(reader);
        variantData = unlit;
        break;
      }
      case materialType::Type::BlinnPhong:
      {
        assets::materialDescriptor::BlinnPhongData blinnPhong;
        blinnPhong.deserialize(reader);
        variantData = blinnPhong;
        break;
      }
      case materialType::Type::Hair:
      {
        assets::materialDescriptor::HairData hair;
        hair.deserialize(reader);
        variantData = hair;
        break;
      }
      default:
        reader.finishReadingObject();
        throw RuntimeErrorException("AMaterialDescriptor::deserialize: Unsupported material type during deserialization.");
    }

    reader.finishReadingObject();
  }

  void MaterialDescriptor::clear()
  {
    name.clear();
    renderMode = materialRenderMode::Type::Opaque;
    alphaCutoutThreshold = 0.5f;
    doubleSided = false;

    std::visit([](auto& variant) {
      variant.clear();
    }, variantData);
  }
}
