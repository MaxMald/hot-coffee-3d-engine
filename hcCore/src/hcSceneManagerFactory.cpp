#include "hc/hcSceneManagerFactory.h"
#include "hc/hcSceneManager.h"
#include "hc/hcGameObjectFactory.h"

namespace hc
{
  namespace SceneManagerFactory
  {
    UniquePtr<SceneManager> create()
    {
      return MakeUnique<SceneManager>(
        MakeUnique<GameObjectFactory>()
      );
    }
  }
}
