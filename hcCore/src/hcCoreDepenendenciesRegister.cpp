#include "hc/hcCoreDependenciesRegister.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32
// include windows platform specific dependencies
#endif

#include "hc/hcSceneManager.h"
#include "hc/hcAssetManager.h"

namespace hc
{
  namespace coreDependenciesRegister
  {
    void registerDependencies(DependencyContainer& dependencyContainer)
    {

#if HC_PLATFORM == HC_PLATFORM_WIN32
      // register windows specific dependencies
#endif

      dependencyContainer.registerInstance<SceneManager>(MakeShared<SceneManager>());
      dependencyContainer.registerInstance<AssetManager>(MakeShared<AssetManager>());
    }
  }
}
