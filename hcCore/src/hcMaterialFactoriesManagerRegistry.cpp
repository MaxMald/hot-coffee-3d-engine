#include "hc/hcMaterialFactoriesManagerRegistry.h"
#include "hc/hcUnlitMaterialFactory.h"
#include "hc/hcMaterialFactoriesManager.h"

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
