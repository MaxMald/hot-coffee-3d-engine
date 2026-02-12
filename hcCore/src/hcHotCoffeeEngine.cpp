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
        "WindowManager is not initialized. Make sure initialize() has been called."
      );
    }
    return *m_windowManager;
  }

  IGraphicsManager& HotCoffeeEngine::getGraphicsManager()
  {
    if (m_graphicsManager == nullptr)
    {
      throw RuntimeErrorException(
        "IGraphicsManager is not initialized. Make sure initialize() has been called."
      );
    }
    return *m_graphicsManager;
  }

  SceneManager& HotCoffeeEngine::getSceneManager()
  {
    if (!m_initialized)
    {
      throw RuntimeErrorException(
        "SceneManager is not initialized. Make sure initialize() has been called."
      );
    }

    return m_sceneManager;
  }

  AssetManager& HotCoffeeEngine::getAssetManager()
  {
    if (!m_initialized)
    {
      throw RuntimeErrorException(
        "AssetManager is not initialized. Make sure initialize() has been called."
      );
    }

    return m_assetManager;
  }

  bool HotCoffeeEngine::isInitialized() const
  {
    return m_initialized;
  }

  ProcessResult HotCoffeeEngine::initialize(const HotCoffeeEngineSettings& settings)
  {
    if (m_initialized)
      return ProcessResult(false, "HotCoffeeEngine is already initialized.");

    try
    {
      LogService::Prepare();
      JsonSerializer::Prepare();

      connectToPlugins(settings.pluginManagerSettings);

      assetManagerLoadersRegistry::registerLoaders(
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
    }
    catch (const std::exception& e)
    {
      LogService::Error(
        "Failed to initialize HotCoffeeEngine: " + String(e.what())
      );

      destroy();
      return ProcessResult(false, "Failed to initialize HotCoffeeEngine.");
    }

    m_initialized = true;
    return ProcessResult();
  }

  void HotCoffeeEngine::destroy()
  {
    m_sceneManager.clear();
    m_assetManager.destroy();

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

    JsonSerializer::Shutdown();
    LogService::Shutdown();

    m_initialized = false;
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
