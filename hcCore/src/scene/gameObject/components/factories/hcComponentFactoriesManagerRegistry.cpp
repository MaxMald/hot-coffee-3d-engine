#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManagerRegistry.h"
#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManager.h"
#include "hc/scene/gameObject/components/factories/hcCameraComponentFactory.h"

#include "hc/scene/hcSceneManager.h"

namespace hc
{
  namespace componentFactoriesManagerRegistry
  {
    void registerFactories(
      ComponentFactoriesManager& manager,
      SceneManager& sceneManager
    )
    {
      manager.registerFactory<CameraComponent>(
        MakeUnique<CameraComponentFactory>(sceneManager)
      );
    }
  }
}
