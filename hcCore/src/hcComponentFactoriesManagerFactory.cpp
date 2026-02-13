#include "hc/hcComponentFactoriesManagerFactory.h"
#include "hc/hcComponentFactoriesManager.h"

namespace hc
{
  UniquePtr<ComponentFactoriesManager> componentFactoriesManagerFactory::create()
  {
    UniquePtr<ComponentFactoriesManager> manager
      = MakeUnique<ComponentFactoriesManager>();

    return manager;
  }
}
