#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManager.h"
#include "hc/scene/gameObject/components/hcMeshComponent.h"
#include "hc/scene/gameObject/components/hcCameraComponent.h"
#include "hc/scene/gameObject/components/hcDirectionalLightComponent.h"
#include "hc/scene/gameObject/components/hcSpotLightComponent.h"
#include "hc/scene/gameObject/components/hcOmniLightComponent.h"

namespace hc
{
  UniquePtr<IComponent> ComponentFactoriesManager::createComponent(
    const componentType::Type& componentType
  ) const
  {
    switch (componentType)
    {
      case componentType::Type::Mesh:
        return createComponent<MeshComponent>();
      case componentType::Type::Camera:
        return createComponent<CameraComponent>();
      case componentType::Type::OmniLight:
        return createComponent<OmniLightComponent>();
      case componentType::Type::SpotLight:
        return createComponent<SpotLightComponent>();
      case componentType::Type::DirectionalLight:
        return createComponent<DirectionalLightComponent>();
      default:
        throw RuntimeErrorException(
          "Component type is unknown or not implemented: " +
          componentType::ToString(componentType)
        );
    }
  }

  void ComponentFactoriesManager::clear()
  {
    m_factories.clear();
  }
}
