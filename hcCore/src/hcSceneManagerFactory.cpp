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
      UniquePtr<SceneManager> sceneManager = MakeUnique<SceneManager>();
      UniquePtr<ComponentFactoriesManager> componentFactoriesManager =
        MakeUnique<ComponentFactoriesManager>();

      componentFactoriesManagerRegistry::registerFactories(
        *componentFactoriesManager,
        *sceneManager
      );

      sceneManager->initialize(
        MakeUnique<GameObjectFactory>(
          std::move(componentFactoriesManager)
        )
      );

      return sceneManager;
    }
  }
}
