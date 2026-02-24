#include "hc/scene/gameObject/hcGameObjectFactory.h"
#include "hc/scene/gameObject/hcGameObject.h"
#include "hc/scene/gameObject/components/hcComponentFactoriesManager.h"

namespace hc
{
  GameObjectFactory::GameObjectFactory(
    UniquePtr<ComponentFactoriesManager> componentFactoriesManager
  ) :
    m_componentFactoriesManager(std::move(componentFactoriesManager))
  {
    if (!m_componentFactoriesManager)
    {
      throw RuntimeErrorException(
        "ComponentFactoriesManager is null."
      );
    }
  }

  GameObjectFactory::~GameObjectFactory()
  { }

  UniquePtr<GameObject> GameObjectFactory::create(const String& name)
  {
    return MakeUnique<GameObject>(
      name,
      *this,
      *m_componentFactoriesManager
    );
  }
}
