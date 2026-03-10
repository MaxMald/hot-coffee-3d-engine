#include "hc/graphics/resource/material/hcMaterialFactoriesManagerFactory.h"
#include "hc/graphics/resource/material/hcUnlitMaterialFactory.h"
#include "hc/graphics/resource/material/hcMaterialFactoriesManager.h"

namespace hc
{
  UniquePtr<MaterialFactoriesManager> MaterialFactoriesManagerFactory::Create()
  {
    UniquePtr<MaterialFactoriesManager> materialFactoriesManager =
      MakeUnique<MaterialFactoriesManager>();

    // Register concrete material factories for supported shading types

    materialFactoriesManager->registerFactory(
      shadingType::Unlit,
      MakeUnique<UnlitMaterialFactory>()
    );

    return materialFactoriesManager;
  }
}
