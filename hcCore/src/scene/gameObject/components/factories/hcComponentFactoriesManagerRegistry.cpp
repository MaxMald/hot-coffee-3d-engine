#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManagerRegistry.h"
#include "hc/scene/gameObject/components/factories/hcComponentFactoriesManager.h"
#include "hc/scene/gameObject/components/factories/hcCameraComponentFactory.h"
#include "hc/scene/gameObject/components/factories/hcMeshComponentFactory.h"
#include "hc/scene/hcSceneManager.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/graphics/hcIGraphicsManager.h"

namespace hc
{
  namespace componentFactoriesManagerRegistry
  {
    void registerFactories(
      ComponentFactoriesManager& manager,
      SceneManager& sceneManager,
      IGraphicsManager& graphicsManager,
      IAssetManager& assetManager
    )
    {
      manager.registerFactory<CameraComponent>(
        MakeUnique<CameraComponentFactory>(sceneManager)
      );
      manager.registerFactory<MeshComponent>(
        MakeUnique<MeshComponentFactory>(graphicsManager.getMeshManager(), assetManager)
      );
    }
  }
}
