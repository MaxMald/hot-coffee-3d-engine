#include "hc/scene/hcSceneManagerFactory.h"
#include "hc/scene/hcSceneManager.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/scene/gameObject/hcGameObjectFactory.h"
#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManager.h"
#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManagerRegistry.h"

namespace hc
{
  namespace SceneManagerFactory
  {
    UniquePtr<SceneManager> create(
      IGraphicsManager& graphicsManager,
      IAssetManager& assetManager
    )
    {
      UniquePtr<SceneManager> sceneManager = MakeUnique<SceneManager>(graphicsManager);
      UniquePtr<ComponentFactoriesManager> componentFactoriesManager =
        MakeUnique<ComponentFactoriesManager>();

      componentFactoriesManagerRegistry::registerFactories(
        *componentFactoriesManager,
        *sceneManager,
        graphicsManager,
        assetManager
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
