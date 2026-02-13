#include "hc/hcSceneManagerFactory.h"
#include "hc/hcSceneManager.h"
#include "hc/hcGameObjectFactory.h"
#include "hc/hcComponentFactoriesManager.h"
#include "hc/hcComponentFactoriesManagerRegistry.h"

namespace hc
{
  namespace SceneManagerFactory
  {
    UniquePtr<SceneManager> create()
    {
      UniquePtr<ComponentFactoriesManager> componentFactoriesManager
        = MakeUnique<ComponentFactoriesManager>();

      ComponentFactoriesManager* componentFactoriesManagerPtr = 
        componentFactoriesManager.get();

      UniquePtr<SceneManager> sceneManager = MakeUnique<SceneManager>(
        MakeUnique<GameObjectFactory>(
          std::move(componentFactoriesManager)
        )
      );

      componentFactoriesManagerRegistry::registerFactories(
        *componentFactoriesManagerPtr,
        *sceneManager
      );

      return sceneManager;
    }
  }
}
