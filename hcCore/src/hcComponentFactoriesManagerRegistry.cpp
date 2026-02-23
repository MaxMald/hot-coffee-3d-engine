#include "hc/scene/gameObject/components/hcComponentFactoriesManagerRegistry.h"
#include "hc/scene/gameObject/components/hcComponentFactoriesManager.h"
#include "hc/scene/gameObject/components/hcCameraComponentFactory.h"

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
