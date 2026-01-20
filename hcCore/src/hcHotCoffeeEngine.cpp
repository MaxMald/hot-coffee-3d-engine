#include "hc/hcHotCoffeeEngine.h"

#include "hc/hcLogService.h"
#include "hc/hcDependencyContainer.h"
#include "hc/hcCoreDependenciesRegister.h"
#include "hc/hcPluginConnectionHelper.h"
#include "hc/hcWindowManager.h"
#include "hc/hcIWindow.h"
#include "hc/hcIGraphicsManager.h"
#include "hc/hcSceneManager.h"
#include "hc/hcAssetManager.h"

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

  SceneManager& HotCoffeeEngine::getSceneManager()
  {
    if (m_sceneManager == nullptr)
    {
      throw RuntimeErrorException(
        "SceneManager is not initialized. Make sure HotCoffeeEngine::start() has been called."
      );
    }

    return *m_sceneManager;
  }

  AssetManager& HotCoffeeEngine::getAssetManager()
  {
    if (m_assetManager == nullptr)
    {
      throw RuntimeErrorException(
        "AssetManager is not initialized. Make sure HotCoffeeEngine::start() has been called."
      );
    }

    return *m_assetManager;
  }

  MaterialManager& HotCoffeeEngine::getMaterialManager()
  {
    if (m_materialManager == nullptr)
    {
      throw RuntimeErrorException(
        "MaterialManager is not initialized. Make sure HotCoffeeEngine::start() has been called."
      );
    }

    return *m_materialManager;
  }

  void HotCoffeeEngine::init(const HotCoffeeEngineSettings& settings)
  {
    if (m_started)
      return;

    m_started = true;

    connectToPlugins(settings.pluginManagerSettings);
    registerDependencies();
    resolveDependencies();
    m_windowManager->createWindow(settings.windowSettings);
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

  void HotCoffeeEngine::connectToPlugins(const PluginManagerSettings& settings)
  {
    m_pluginManager.init();
    pluginConnectionHelper::connectToPluginsFromSettings(
      m_pluginManager,
      settings
    );
  }

  void HotCoffeeEngine::registerDependencies()
  {
    coreDependenciesRegister::registerDependencies(m_dependencyContainer);
    m_pluginManager.addDependenciesFromPlugins(m_dependencyContainer);
  }

  void HotCoffeeEngine::resolveDependencies()
  {
    m_dependencyContainer.resolveAllDependencies();

    m_windowManager = m_dependencyContainer.resolve<WindowManager>();
    m_graphicsManager = m_dependencyContainer.resolve<IGraphicsManager>();
    m_sceneManager = m_dependencyContainer.resolve<SceneManager>();
    m_assetManager = m_dependencyContainer.resolve<AssetManager>();
  }

  HotCoffeeEngine::HotCoffeeEngine() :
    m_started(false)
  {
  }

  HotCoffeeEngine::~HotCoffeeEngine()
  {
  }
}
