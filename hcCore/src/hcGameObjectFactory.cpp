#include "hc/hcGameObjectFactory.h"
#include "hc/hcGameObject.h"
#include "hc/hcComponentFactoriesManager.h"

namespace hc
{
  GameObjectFactory::GameObjectFactory(
    UniquePtr<ComponentFactoriesManager> componentFactoriesManager
  ) :
    m_componentFactoriesManager(std::move(componentFactoriesManager))
  {
  }

  UniquePtr<GameObject> GameObjectFactory::create(const String& name)
  {
    return MakeUnique<GameObject>(
      name,
      *this,
      *m_componentFactoriesManager
    );
  }

  ComponentFactoriesManager& GameObjectFactory::getComponentFactoriesManager() const
  {
    if (!m_componentFactoriesManager)
    {
      throw InvalidArgumentException(
        "ComponentFactoriesManager pointer is null."
      );
    }

    return *m_componentFactoriesManager;
  }
}
