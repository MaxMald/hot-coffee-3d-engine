#include "hc/hcComponentFactoriesManagerRegistry.h"
#include "hc/hcComponentFactoriesManager.h"
#include "hc/hcCameraComponentFactory.h"

#include "hc/hcSceneManager.h"

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
