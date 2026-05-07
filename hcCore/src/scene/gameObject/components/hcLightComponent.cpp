#include "hc/scene/gameObject/components/hcLightComponent.h"
#include "hc/scene/light/hcLight.h"
#include "hc/scene/light/hcLightManager.h"

namespace hc
{
  LightComponent::LightComponent(LightManager& lightManager) :
    ABaseComponent(componentType::Type::Light),
    m_light(nullptr),
    m_lightManager(lightManager)
  {
  }

  LightComponent::~LightComponent()
  {
    if (m_light)
      m_lightManager.destroyLight(m_light->getUUID());
  }

  void LightComponent::create()
  {
    if (m_light)
      return;

    m_light = m_lightManager.createLight();
  }

  Light* LightComponent::getLight() const
  {
    return m_light;
  }

  bool LightComponent::hasLight() const
  {
    return m_light != nullptr;
  }

  void LightComponent::destroyLight()
  {
    if (!m_light)
      return;

    m_lightManager.destroyLight(m_light->getUUID());
    m_light = nullptr;
  }

  void LightComponent::onSerialize(BinaryWriter& writer) const
  {
    bool hasLight = (m_light != nullptr);
    writer.writeBool(hasLight);
    if (!hasLight)
      return;

    m_light->serialize(writer);
  }

  void LightComponent::onDeserialize(BinaryReader& reader)
  {
    if (m_light)
    {
      m_lightManager.destroyLight(m_light->getUUID());
      m_light = nullptr;
    }

    bool hasLight = reader.readBool();
    if (!hasLight)
      return;

    m_light = m_lightManager.deserializeLight(reader);
  }
}
