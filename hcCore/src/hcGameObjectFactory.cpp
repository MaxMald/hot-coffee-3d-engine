#include "hc/hcGameObjectFactory.h"
#include "hc/hcComponentFactoriesManager.h"
#include "hc/hcGameObject.h"

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
      *this,
      *m_componentFactoriesManager,
      name
    );
  }
}
