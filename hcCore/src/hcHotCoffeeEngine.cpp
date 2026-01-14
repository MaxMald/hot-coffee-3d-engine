#include "hc/hcHotCoffeeEngine.h"

#include "hc/hcDependencyContainer.h"
#include "hc/hcCoreDependenciesRegister.h"

namespace hc
{
  HotCoffeeEngine* HotCoffeeEngine::_Instance = nullptr;

  HotCoffeeEngine& HotCoffeeEngine::Instance()
  {
    if (_Instance == nullptr)
    {
      throw RuntimeErrorException(
        "HotCoffeeEngine instance is not prepared. Call HotCoffeeEngine::Prepare() before accessing the instance."
      );
    }
    return *_Instance;
  }

  void HotCoffeeEngine::Prepare()
  {
    if (_Instance == nullptr)
    {
      _Instance = new HotCoffeeEngine();
      _Instance->onPrepare();
    }
  }

  void HotCoffeeEngine::Shutdown()
  {
    if (_Instance != nullptr)
    {
      _Instance->onShutdown();
      _Instance = nullptr;
    }
  }

  void HotCoffeeEngine::start()
  {

  }

  void HotCoffeeEngine::onPrepare()
  {
    prepareAndResolveDependencyContainer();
  }

  void HotCoffeeEngine::onShutdown()
  {
  }

  void HotCoffeeEngine::prepareAndResolveDependencyContainer()
  {
    DependencyContainer dependencyContainer;
    coreDependenciesRegister::registerDependencies(dependencyContainer);
    // add more dependencies here if needed
    
    dependencyContainer.resolveAllDependencies();
  }

  HotCoffeeEngine::HotCoffeeEngine()
  {
  }

  HotCoffeeEngine::~HotCoffeeEngine()
  {
  }
}
