#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManager.h"
#include "hc/scene/gameObject/components/hcMeshComponent.h"
#include "hc/scene/gameObject/components/hcCameraComponent.h"

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
      default:
        return nullptr;
    }
  }

  void ComponentFactoriesManager::clear()
  {
    m_factories.clear();
  }
}
