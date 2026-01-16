#include "hc/hcHotCoffeeEngine.h"

#include "hc/hcLogService.h"
#include "hc/hcDependencyContainer.h"
#include "hc/hcCoreDependenciesRegister.h"
#include "hc/hcPluginConnectionHelper.h"
#include "hc/hcWindowManager.h"
#include "hc/hcIWindow.h"

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

  void HotCoffeeEngine::start(const HotCoffeeEngineSettings& settings)
  {
    if (m_started)
      return;

    m_started = true;

    m_pluginManager.init();
    pluginConnectionHelper::connectToPluginsFromSettings(
      m_pluginManager,
      settings.pluginManagerSettings
    );

    prepareAndResolveDependencyContainer();

    // Window Creation

    m_windowManager->createWindow(settings.windowSettings);
    SharedPtr<IWindow> window = m_windowManager->getWindow();

    // Game Loop

    while (window->isOpen())
    {
      while (Optional<Event> event = window->pollEvent())
      {
        if (event->is<Event::Closed>())
        {
          window->destroy();
        }
      }
    }
  }

  void HotCoffeeEngine::onPrepare()
  {
    LogService::Prepare();
  }

  void HotCoffeeEngine::onShutdown()
  {
    m_dependencyContainer.clear();
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
  }

  HotCoffeeEngine::HotCoffeeEngine() :
    m_started(false)
  {
  }

  HotCoffeeEngine::~HotCoffeeEngine()
  {
  }
}
