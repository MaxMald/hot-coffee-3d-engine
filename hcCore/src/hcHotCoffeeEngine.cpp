#include "hc/hcHotCoffeeEngine.h"

#include "hc/hcLogService.h"
#include "hc/hcDependencyContainer.h"
#include "hc/hcCoreDependenciesRegister.h"
#include "hc/hcPluginConnectionHelper.h"

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

  const PluginManager& HotCoffeeEngine::getPluginManager() const
  {
    return m_pluginManager;
  }

  void HotCoffeeEngine::start()
  {
    if (m_started)
      return;

    m_started = true;

    m_pluginManager.init();
    pluginConnectionHelper::connectWindowSfmlPlugin(m_pluginManager);

    prepareAndResolveDependencyContainer();
  }

  void HotCoffeeEngine::onPrepare()
  {
    LogService::Prepare();
  }

  void HotCoffeeEngine::onShutdown()
  {
    // Clean up resources here if needed
    m_pluginManager.closeAll();
    LogService::Shutdown();
  }

  void HotCoffeeEngine::prepareAndResolveDependencyContainer()
  {
    coreDependenciesRegister::registerDependencies(m_dependencyContainer);
    m_pluginManager.addDependenciesFromPlugins(m_dependencyContainer);

    m_dependencyContainer.resolveAllDependencies();
  }

  HotCoffeeEngine::HotCoffeeEngine() :
    m_started(false)
  {
  }

  HotCoffeeEngine::~HotCoffeeEngine()
  {
  }
}
