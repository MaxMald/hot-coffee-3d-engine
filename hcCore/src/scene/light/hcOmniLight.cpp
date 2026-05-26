#include "hc/scene/light/hcOmniLight.h"

namespace hc
{
  OmniLight::OmniLight() :
    ALight(lightType::Type::Omni)
  {
  }

  OmniLightFrameData OmniLight::toFrameData() const
  {
    OmniLightFrameData frameData{};
    frameData.position = Vector4f(m_position, 1.0f);
    frameData.color = m_color;
    frameData.range = m_range;
    frameData.intensity = m_intensity;
    return frameData;
  }
}
