#include "hc/scene/light/hcOmniLight.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc
{
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
}
