#include "hc/scene/gameObject/components/hcLightComponent.h"
#include "hc/scene/gameObject/hcGameObject.h"

namespace hc
{
  static constexpr UInt16 LIGHT_COMPONENT_VERSION = 1;

  LightComponent::LightComponent() :
    ABaseComponent(componentType::Light),
    m_light()
  {
  }

  LightComponent::~LightComponent() = default;

  void LightComponent::preUpdate(float)
  {
  }

  void LightComponent::update(float)
  {
  }

  void LightComponent::postUpdate(float deltaTime)
  {
    updateLight();
  }

  void LightComponent::updateLight()
  {
    m_light.position = getGameObject()->getWorldPosition();
    scene::lights::lightType::Type lightType = m_light.getType();

    if (lightType == scene::lights::lightType::Directional)
    {
      scene::lights::DirectionalLight* dirLight = m_light.getIfDirectionalLight();
      if (!dirLight)
        throw RuntimeErrorException("LightComponent: Light is not a DirectionalLight.");

      Matrix4 worldRotation = m_gameObject->getWorldRotationMatrix();
      Vector3f direction = Vector3f(0.0f, 0.0f, -1.0f);
      dirLight->setDirection((worldRotation * Vector4f(direction, 0.0f)).xyz());
    }
    else if (lightType == scene::lights::lightType::Spot)
    {
      scene::lights::SpotLight* spotLight = m_light.getIfSpotLight();
      if (!spotLight)
        throw RuntimeErrorException("LightComponent: Light is not a SpotLight.");

      Matrix4 worldRotation = m_gameObject->getWorldRotationMatrix();
      Vector3f direction = Vector3f(0.0f, 0.0f, -1.0f);
      spotLight->setDirection((worldRotation * Vector4f(direction, 0.0f)).xyz());
    }
    else if (lightType == scene::lights::lightType::Omni)
    {
      // Omni lights do not have a direction, so no need to update direction.
    }
    else
    {
      throw InvalidArgumentException("LightComponent: Unsupported light type in LightComponent::updateLight.");
    }
  }

  void LightComponent::setLightType(scene::lights::lightType::Type type)
  {
    if (m_light.getType() == type)
      return;
    m_light.setType(type);
  }

  void LightComponent::onSerialize(io::BinaryWriter& writer) const
  {
    m_light.serialize(writer);
  }

  void LightComponent::onDeserialize(io::BinaryReader& reader)
  {
    m_light.deserialize(reader);
  }

  UInt16 LightComponent::getDerivedVersion() const
  {
    return LIGHT_COMPONENT_VERSION;
  }
}
