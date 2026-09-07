#include "hc/scene/light/hcOmniLight.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc
{
  static constexpr UInt16 OMNI_LIGHT_VERSION = 1;

  OmniLight::OmniLight() :
    ALight(lightType::Type::Omni)
  {}

  dataBlockStructure::OmniLight OmniLight::getDataBlockStructure() const
  {
    dataBlockStructure::OmniLight data;
    data.position = Vector4f(m_position, 1.0f);
    data.color = m_color;
    data.range = m_range;
    data.intensity = m_intensity;
    return data;
  }

  void OmniLight::onSerialize(io::BinaryWriter&) const
  {
    // Intentionally left empty as there are no additional members to serialize in this
    // derived class.
  }

  void OmniLight::onDeserialize(io::BinaryReader&)
  {
    // Intentionally left empty as there are no additional members to deserialize in this
    // derived class.
  }

  UInt16 OmniLight::getDerivedVersion() const
  {
    return OMNI_LIGHT_VERSION;
  }
}
