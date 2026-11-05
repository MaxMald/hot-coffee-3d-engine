#include "hc/scene/light/hcLight.h"

namespace hc::scene::lights
{
  // ---- OmniLight Implementation

  static constexpr UInt32 OMNI_LIGHT_VERSION = 1;

  void OmniLight::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(lightType::Type::Omni), OMNI_LIGHT_VERSION);
    writer.writeFloat(range);
    writer.finishWritingObject();
  }

  void OmniLight::deserialize(io::BinaryReader& reader)
  {
    *this = OmniLight();
    io::ObjectHeader header = reader.startReadingObject();
    if (!header.match(static_cast<UInt32>(lightType::Type::Omni), OMNI_LIGHT_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    range = reader.readFloat();
    reader.finishReadingObject();
  }

  // ---- DirectionalLight Implementation

  static constexpr UInt32 DIRECTIONAL_LIGHT_VERSION = 1;

  void DirectionalLight::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(lightType::Type::Directional), DIRECTIONAL_LIGHT_VERSION);
    writer.writeVector3f(direction);
    writer.writeVector3f(shadowViewTarget);
    writer.writeFloat(shadowViewDistance);
    writer.writeFloat(shadowProjectionSize);
    writer.writeFloat(shadowProjectionNearPlane);
    writer.writeFloat(shadowProjectionFarPlane);
    writer.finishWritingObject();
  }

  void DirectionalLight::deserialize(io::BinaryReader& reader)
  {
    *this = DirectionalLight();
    io::ObjectHeader header = reader.startReadingObject();
    if (!header.match(static_cast<UInt32>(lightType::Type::Directional), DIRECTIONAL_LIGHT_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    direction = reader.readVector3f();
    shadowViewTarget = reader.readVector3f();
    shadowViewDistance = reader.readFloat();
    shadowProjectionSize = reader.readFloat();
    shadowProjectionNearPlane = reader.readFloat();
    shadowProjectionFarPlane = reader.readFloat();
    reader.finishReadingObject();
  }

  // ---- SpotLight Implementation

  static constexpr UInt32 SPOT_LIGHT_VERSION = 1;

  void SpotLight::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(lightType::Type::Spot), SPOT_LIGHT_VERSION);
    writer.writeVector3f(direction);
    writer.writeAngle(innerConeAngle);
    writer.writeAngle(outerConeAngle);
    writer.writeFloat(shadowProjectionNearPlane);
    writer.writeFloat(shadowProjectionFarPlane);
    writer.writeFloat(range);
    writer.finishWritingObject();
  }

  void SpotLight::deserialize(io::BinaryReader& reader)
  {
    *this = SpotLight();
    io::ObjectHeader header = reader.startReadingObject();
    if (!header.match(static_cast<UInt32>(lightType::Type::Spot), SPOT_LIGHT_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    direction = reader.readVector3f();
    innerConeAngle = reader.readAngle();
    outerConeAngle = reader.readAngle();
    shadowProjectionNearPlane = reader.readFloat();
    shadowProjectionFarPlane = reader.readFloat();
    range = reader.readFloat();
    reader.finishReadingObject();
  }

  // ---- Light Implementation

  static constexpr UInt32 LIGHT_VERSION = 1;

  Light::Light() :
    m_variantData(OmniLight()),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    position(0.0f, 0.0f, 0.0f),
    intensity(1.0f),
    shadowBias(0.005f),
    shadowStrength(1.0f),
    enabled(true),
    shadowsEnabled(false),
    m_type(lightType::Type::Omni)
  {
  }

  Light::~Light() = default;

  void Light::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(m_type), LIGHT_VERSION);
    writer.writeColor(color);
    writer.writeVector3f(position);
    writer.writeFloat(intensity);
    writer.writeFloat(shadowBias);
    writer.writeFloat(shadowStrength);
    writer.writeBool(enabled);
    writer.writeBool(shadowsEnabled);

    std::visit([&writer](const auto& lightData) {
      lightData.serialize(writer);
      }, m_variantData);

    writer.finishWritingObject();
  }

  void Light::deserialize(io::BinaryReader& reader)
  {
    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchVersion(LIGHT_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    m_type = static_cast<lightType::Type>(header.type);
    color = reader.readColor();
    position = reader.readVector3f();
    intensity = reader.readFloat();
    shadowBias = reader.readFloat();
    shadowStrength = reader.readFloat();
    enabled = reader.readBool();
    shadowsEnabled = reader.readBool();

    switch (m_type)
    {
    case lightType::Type::Omni:
      m_variantData = OmniLight();
      break;
    case lightType::Type::Directional:
      m_variantData = DirectionalLight();
      break;
    case lightType::Type::Spot:
      m_variantData = SpotLight();
      break;
    default:
      reader.finishReadingObject();
      throw RuntimeErrorException("Light::deserialize: Unsupported light type during deserialization.");
    }

    std::visit([&reader](auto& lightData) {
      lightData.deserialize(reader);
      }, m_variantData);

    reader.finishReadingObject();
  }

  void Light::setType(lightType::Type lightType)
  { 
    switch (lightType)
    {
    case lightType::Type::Omni:
      m_variantData = OmniLight();
      break;
    case lightType::Type::Directional:
      m_variantData = DirectionalLight();
      break;
    case lightType::Type::Spot:
      m_variantData = SpotLight();
      break;
    default:
      throw RuntimeErrorException("Light::setType: Unsupported light type during type setting.");
    }
    m_type = lightType;
  }

  void Light::clear()
  {
    color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    position = Vector3f(0.0f, 0.0f, 0.0f);
    intensity = 1.0f;
    shadowBias = 0.005f;
    shadowStrength = 1.0f;
    enabled = true;
    shadowsEnabled = false;

    switch (m_type)
    {
    case lightType::Type::Omni:
      m_variantData = OmniLight();
      break;
    case lightType::Type::Directional:
      m_variantData = DirectionalLight();
      break;
    case lightType::Type::Spot:
      m_variantData = SpotLight();
      break;
    default:
      throw RuntimeErrorException("Light::clear: Unimplemented light type during clearing.");
    }
  }

  dataBlockStructure::OmniLight Light::getOmniLightDataBlockStructure() const
  {
    const scene::lights::OmniLight* omniLight
      = std::get_if<scene::lights::OmniLight>(&m_variantData);
    if (omniLight == nullptr)
      throw RuntimeErrorException(
        "Light::getOmniLightDataBlockStructure: Current light type is not an OmniLight."
      );

    dataBlockStructure::OmniLight data;
    data.position = Vector4f(position.x, position.y, position.z, 0.0f);
    data.color = color;
    data.range = omniLight->range;
    data.intensity = intensity;

    return data;
  }

  dataBlockStructure::DirectionalLight Light::getDirectionalLightDataBlockStructure() const
  {
    const scene::lights::DirectionalLight* directionalLight
      = std::get_if<scene::lights::DirectionalLight>(&m_variantData);
    if (directionalLight == nullptr)
      throw RuntimeErrorException(
        "Light::getDirectionalLightDataBlockStructure: Current light type is not a DirectionalLight."
      );

    dataBlockStructure::DirectionalLight data;
    data.directionAndIntensity = Vector4f(
      directionalLight->direction.x,
      directionalLight->direction.y,
      directionalLight->direction.z,
      intensity
    );
    data.color = color;

    return data;
  }

  dataBlockStructure::SpotLight Light::getSpotLightDataBlockStructure() const
  {
    const scene::lights::SpotLight* spotLight
      = std::get_if<scene::lights::SpotLight>(&m_variantData);
    if (spotLight == nullptr)
      throw RuntimeErrorException(
        "Light::getSpotLightDataBlockStructure: Current light type is not a SpotLight."
      );

    dataBlockStructure::SpotLight data;
    data.position = Vector4f(position.x, position.y, position.z, 0.0f);
    data.direction = Vector4f(
      spotLight->direction.x,
      spotLight->direction.y,
      spotLight->direction.z,
      0.0f
    );
    data.color = color;
    data.range = spotLight->range;
    data.innerConeAngle = Math::Cos(spotLight->innerConeAngle.toRadians());
    data.intensity = intensity;
    data.outerConeAngle = Math::Cos(spotLight->outerConeAngle.toRadians());

    return data;
  }
}
