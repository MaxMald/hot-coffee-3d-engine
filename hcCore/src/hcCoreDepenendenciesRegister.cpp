#include "hc/hcCoreDependenciesRegister.h"
#include "hc/hcDependencyContainer.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32
// include windows platform specific dependencies
#endif

#include "hc/hcWindowManager.h"
#include "hc/hcSceneManager.h"

namespace hc
{
  namespace coreDependenciesRegister
  {
    void registerDependencies(DependencyContainer& dependencyContainer)
    {

#if HC_PLATFORM == HC_PLATFORM_WIN32
      // register windows specific dependencies
#endif

      dependencyContainer.registerInstance<WindowManager>(MakeShared<WindowManager>());
      dependencyContainer.registerInstance<SceneManager>(MakeShared<SceneManager>());
    }
  }
}
