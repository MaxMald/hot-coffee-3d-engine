#include "hc/scene/hcSceneManagerFactory.h"
#include "hc/scene/hcSceneManager.h"
#include "hc/scene/gameObject/hcGameObjectFactory.h"
#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManager.h"
#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManagerRegistry.h"

namespace hc
{
  namespace SceneManagerFactory
  {
    UniquePtr<SceneManager> create(IGraphicsManager& graphicsManager)
    {
      UniquePtr<SceneManager> sceneManager = MakeUnique<SceneManager>();
      UniquePtr<ComponentFactoriesManager> componentFactoriesManager =
        MakeUnique<ComponentFactoriesManager>();

      componentFactoriesManagerRegistry::registerFactories(
        *componentFactoriesManager,
        *sceneManager,
        graphicsManager
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
