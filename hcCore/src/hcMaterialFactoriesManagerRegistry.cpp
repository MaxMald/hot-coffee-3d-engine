#include "hc/graphics/resource/material/hcMaterialFactoriesManagerRegistry.h"
#include "hc/graphics/resource/material/hcUnlitMaterialFactory.h"
#include "hc/graphics/resource/material/hcMaterialFactoriesManager.h"

namespace hc
{
  namespace materialFactoriesManagerRegistry
  {
    void resigtryMaterialFactories(
      MaterialFactoriesManager& materialFactoriesManager
    )
    {
      materialFactoriesManager.registerFactory(
        shadingType::Unlit,
        MakeUnique<UnlitMaterialFactory>()
      );
    }
  }
}
