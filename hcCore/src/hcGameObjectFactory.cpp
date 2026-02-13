#include "hc/hcGameObjectFactory.h"
#include "hc/hcGameObject.h"

namespace hc
{
  GameObjectFactory::GameObjectFactory()
  {
  }

  UniquePtr<GameObject> GameObjectFactory::create(const String& name)
  {
    return MakeUnique<GameObject>(
      name,
      *this
    );
  }
}
