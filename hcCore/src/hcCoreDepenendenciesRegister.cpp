#include "hc/hcCoreDependenciesRegister.h"
#include "hc/hcDependencyContainer.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32
#include "hc/hcWindowsPluginSlotFactory.h"
#endif

#include "hc/hcPluginManager.h"

namespace hc
{
  namespace coreDependenciesRegister
  {
    void registerDependencies(DependencyContainer& dependencyContainer)
    {

#if HC_PLATFORM == HC_PLATFORM_WIN32
      dependencyContainer.registerType<WindowsPluginSlotFactory>();
#endif

      dependencyContainer.registerType<PluginManager>();
    }
  }
}
