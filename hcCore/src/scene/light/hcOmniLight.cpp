#include "hc/scene/light/hcOmniLight.h"

namespace hc
{
  OmniLight::OmniLight() :
    ALight(lightType::Type::Omni),
    m_range(10.0f)
  {
  }

  void OmniLight::serialize(BinaryWriter& writer) const
  {
    ALight::serialize(writer);
    writer.writeFloat(m_range);
  }

  void OmniLight::deserialize(BinaryReader& reader)
  {
    ALight::deserialize(reader);
    m_range = reader.readFloat();
  }

  void OmniLight::setRange(float range)
  {
    m_range = range;
  }

  float OmniLight::getRange() const
  {
    return m_range;
  }
}
