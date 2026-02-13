#include "hc/hcSceneManagerFactory.h"
#include "hc/hcSceneManager.h"
#include "hc/hcGameObjectFactory.h"
#include "hc/hcComponentFactoriesManager.h"
#include "hc/hcComponentFactoriesManagerFactory.h"

namespace hc
{
  namespace SceneManagerFactory
  {
    UniquePtr<SceneManager> create()
    {
      return MakeUnique<SceneManager>(
        MakeUnique<GameObjectFactory>(
          componentFactoriesManagerFactory::create()
        )
      );
    }
  }
}
