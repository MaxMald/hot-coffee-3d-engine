#include "hc/scene/hcSceneManagerFactory.h"
#include "hc/scene/hcSceneManager.h"
#include "hc/scene/gameObject/hcGameObjectFactory.h"
#include "hc/scene/gameObject/components/hcComponentFactoriesManager.h"
#include "hc/scene/gameObject/components/hcComponentFactoriesManagerRegistry.h"

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
