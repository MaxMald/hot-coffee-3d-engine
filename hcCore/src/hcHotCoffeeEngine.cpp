#include "hc/hcHotCoffeeEngine.h"

#include "hc/hcLogService.h"
#include "hc/hcDependencyContainer.h"
#include "hc/hcCoreDependenciesRegister.h"
#include "hc/hcPluginConnectionHelper.h"
#include "hc/hcWindowManager.h"
#include "hc/hcIWindow.h"
#include "hc/hcIGraphicsManager.h"

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

  WindowManager& HotCoffeeEngine::getWindowManager()
  {
    if (m_windowManager == nullptr)
    {
      throw RuntimeErrorException(
        "WindowManager is not initialized. Make sure HotCoffeeEngine::start() has been called."
      );
    }

    return *m_windowManager;
  }

  IGraphicsManager& HotCoffeeEngine::getGraphicsManager()
  {
    if (m_graphicsManager == nullptr)
    {
      throw RuntimeErrorException(
        "IGraphicsManager is not initialized. Make sure HotCoffeeEngine::start() has been called."
      );
    }

    return *m_graphicsManager;
  }

  void HotCoffeeEngine::init(const HotCoffeeEngineSettings& settings)
  {
    if (m_started)
      return;

    m_started = true;

    // Plugin Manager Init
    m_pluginManager.init();
    pluginConnectionHelper::connectToPluginsFromSettings(
      m_pluginManager,
      settings.pluginManagerSettings
    );

    // Resolve Dependencies
    prepareAndResolveDependencyContainer();

    // Window Creation
    m_windowManager->createWindow(settings.windowSettings);

    // Graphics Init
    m_graphicsManager->init();
  }

  void HotCoffeeEngine::onPrepare()
  {
    LogService::Prepare();
  }

  void HotCoffeeEngine::onShutdown()
  {
    m_dependencyContainer.clear();
    m_graphicsManager->destroy();
    m_graphicsManager = nullptr;
    m_windowManager = nullptr;
    m_pluginManager.closeAll();
    LogService::Shutdown();
  }

  void HotCoffeeEngine::prepareAndResolveDependencyContainer()
  {
    coreDependenciesRegister::registerDependencies(m_dependencyContainer);
    m_pluginManager.addDependenciesFromPlugins(m_dependencyContainer);

    m_dependencyContainer.resolveAllDependencies();

    m_windowManager = m_dependencyContainer.resolve<WindowManager>();
    m_graphicsManager = m_dependencyContainer.resolve<IGraphicsManager>();
  }

  HotCoffeeEngine::HotCoffeeEngine() :
    m_started(false)
  {
  }

  HotCoffeeEngine::~HotCoffeeEngine()
  {
  }
}
