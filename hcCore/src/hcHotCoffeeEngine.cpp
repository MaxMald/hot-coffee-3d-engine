#include "hc/hcHotCoffeeEngine.h"

#include "hc/hcJsonSerializer.h"
#include "hc/hcCoreDependenciesRegister.h"
#include "hc/hcPluginConnectionHelper.h"
#include "hc/hcIWindowManager.h"
#include "hc/hcIWindow.h"
#include "hc/hcIGraphicsManager.h"
#include "hc/hcSceneManager.h"
#include "hc/hcAssetManager.h"

#include "hc/hcGraphicsManagerFactory.h"

namespace hc
{
  void HotCoffeeEngine::Initialize(const HotCoffeeEngineSettings& settings)
  {
    HotCoffeeEngine::Instance().initialize(settings);
  }

  IGraphicsManager& HotCoffeeEngine::GetGraphicsManager()
  {
    return HotCoffeeEngine::Instance().getGraphicsManager();
  }

  SceneManager& HotCoffeeEngine::GetSceneManager()
  {
    return HotCoffeeEngine::Instance().getSceneManager();
  }

  AssetManager& HotCoffeeEngine::GetAssetManager()
  {
    return HotCoffeeEngine::Instance().getAssetManager();
  }

  IWindowManager& HotCoffeeEngine::GetWindowManager()
  {
    return HotCoffeeEngine::Instance().getWindowManager();
  }

  HotCoffeeEngine::HotCoffeeEngine() :
    m_initialized(false)
  {
  }

  HotCoffeeEngine::~HotCoffeeEngine()
  {
  }

  const PluginManager& HotCoffeeEngine::getPluginManager() const
  {
    return m_pluginManager;
  }

  IWindowManager& HotCoffeeEngine::getWindowManager()
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

  void HotCoffeeEngine::initialize(const HotCoffeeEngineSettings& settings)
  {
    if (m_initialized)
      return;

    m_initialized = true;

    connectToPlugins(settings.pluginManagerSettings);
    registerDependencies();
    resolveDependencies();
    m_windowManager->createWindow(settings.windowSettings);

    m_graphicsManager = graphicsManagerFactory::Create(
      m_pluginManager,
      m_windowManager->getWindow(),
      *m_assetManager
    );
    m_graphicsManager->initialize();
  }

  void HotCoffeeEngine::onPrepare()
  {
    LogService::Prepare();
    JsonSerializer::Prepare();
  }

  void HotCoffeeEngine::onShutdown()
  {
    if (m_initialized)
    {
      m_sceneManager->destroy();
      m_graphicsManager->destroy();
      m_graphicsManager.reset();
      m_windowManager = nullptr;
      m_assetManager->clear();
      m_assetManager = nullptr;
      m_dependencyContainer.clear();
      m_pluginManager.closeAll();

      m_initialized = false;
    }

    JsonSerializer::Shutdown();
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

    m_windowManager = m_dependencyContainer.resolve<IWindowManager>();
    m_sceneManager = m_dependencyContainer.resolve<SceneManager>();
    m_assetManager = m_dependencyContainer.resolve<AssetManager>();
  }
}
