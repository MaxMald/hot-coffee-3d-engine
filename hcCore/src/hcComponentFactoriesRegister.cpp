#include "hc/hcComponentFactoriesRegister.h"
#include "hc/hcComponentFactoriesManager.h"

#include "hc/hcCameraComponentFactory.h"

namespace hc
{
  namespace componentFactoriesRegister
  {
    void registerComponentFactories(
      ComponentFactoriesManager& factoryManager,
      SceneManager& sceneManager
    )
    {
      factoryManager.registerComponentFactory<CameraComponent>(
        MakeUnique<CameraComponentFactory>(sceneManager)
      );
    }
  }
}
