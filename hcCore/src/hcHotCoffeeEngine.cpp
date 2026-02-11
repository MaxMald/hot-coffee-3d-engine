#include "hc/hcHotCoffeeEngine.h"

#include "hc/hcJsonSerializer.h"
#include "hc/hcPluginConnectionHelper.h"
#include "hc/hcIWindowManager.h"
#include "hc/hcIWindow.h"
#include "hc/hcIGraphicsManager.h"

#include "hc/hcGraphicsManagerFactory.h"
#include "hc/hcWindowManagerFactory.h"
#include "hc/hcAssetManagerLoadersRegistry.h"

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
    m_graphicsManager(nullptr),
    m_windowManager(nullptr),
    m_sceneManager(),
    m_assetManager(),
    m_pluginManager(),
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
        "WindowManager is not initialized. Make sure HotCoffeeEngine::initialize() has been called."
      );
    }
    return *m_windowManager;
  }

  IGraphicsManager& HotCoffeeEngine::getGraphicsManager()
  {
    if (m_graphicsManager == nullptr)
    {
      throw RuntimeErrorException(
        "IGraphicsManager is not initialized. Make sure HotCoffeeEngine::initialize() has been called."
      );
    }
    return *m_graphicsManager;
  }

  SceneManager& HotCoffeeEngine::getSceneManager()
  {
    if (!m_initialized)
    {
      throw RuntimeErrorException(
        "SceneManager is not initialized. Make sure HotCoffeeEngine::initialize() has been called."
      );
    }

    return m_sceneManager;
  }

  AssetManager& HotCoffeeEngine::getAssetManager()
  {
    if (!m_initialized)
    {
      throw RuntimeErrorException(
        "AssetManager is not initialized. Make sure HotCoffeeEngine::initialize() has been called."
      );
    }

    return m_assetManager;
  }

  void HotCoffeeEngine::initialize(const HotCoffeeEngineSettings& settings)
  {
    if (m_initialized)
      return;

    connectToPlugins(settings.pluginManagerSettings);

    assetManagerLoadersRegistry::RegisterLoaders(
      m_assetManager,
      m_pluginManager
    );

    m_windowManager = windowManagerFactory::Create(m_pluginManager);
    m_windowManager->createWindow(settings.windowSettings);

    m_graphicsManager = graphicsManagerFactory::Create(
      m_pluginManager,
      m_windowManager->getWindow(),
      m_assetManager
    );
    m_graphicsManager->initialize();

    m_initialized = true;
  }

  void HotCoffeeEngine::onPrepare()
  {
    LogService::Prepare();
    JsonSerializer::Prepare();
  }

  void HotCoffeeEngine::onShutdown()
  {
    m_sceneManager.clear();
    m_assetManager.clear();

    if (m_graphicsManager)
    {
      m_graphicsManager->destroy();
      m_graphicsManager.reset();
    }

    if (m_windowManager)
    {
      m_windowManager->destroy();
      m_windowManager.reset();
    }

    m_pluginManager.closeAll();

    m_initialized = false;

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
}
