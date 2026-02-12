#include "hc/hcSceneManagerFactory.h"
#include "hc/hcSceneManager.h"
#include "hc/hcGameObjectFactory.h"
#include "hc/hcComponentFactoriesManager.h"
#include "hc/hcComponentFactoriesRegister.h"

namespace hc
{
  namespace sceneManagerFactory
  {
    UniquePtr<SceneManager> create()
    {
      UniquePtr<ComponentFactoriesManager> componentsFactoriesManager =
        MakeUnique<ComponentFactoriesManager>();
      ComponentFactoriesManager* componentsFactoriesManagerPtr = componentsFactoriesManager.get();

      UniquePtr<GameObjectFactory> gameObjectFactory = MakeUnique<GameObjectFactory>(
        std::move(componentsFactoriesManager)
      );

      UniquePtr<SceneManager> sceneManager = MakeUnique<SceneManager>(
        std::move(gameObjectFactory)
      );

      componentFactoriesRegister::registerComponentFactories(
        *componentsFactoriesManagerPtr,
        *sceneManager
      );

      return std::move(sceneManager);
    }
  }
}
