#include "hc/hcHotCoffeeEngine.h"

#include "hc/hcJsonSerializer.h"
#include "hc/hcPluginConnectionHelper.h"
#include "hc/hcIWindowManager.h"
#include "hc/hcIWindow.h"
#include "hc/hcIGraphicsManager.h"
#include "hc/hcSceneManager.h"
#include "hc/hcIEventListener.h"

#include "hc/hcGraphicsManagerFactory.h"
#include "hc/hcWindowManagerFactory.h"
#include "hc/hcAssetManagerLoadersRegistry.h"
#include "hc/hcSceneManagerFactory.h"

namespace hc
{
  HotCoffeeEngine::HotCoffeeEngine() :
    m_graphicsManager(nullptr),
    m_windowManager(nullptr),
    m_sceneManager(nullptr),
    m_assetManager(),
    m_pluginManager(),
    m_frameClock(),
    m_eventListeners(),
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
    assertEngineIsInitialized();
    return *m_windowManager;
  }

  IGraphicsManager& HotCoffeeEngine::getGraphicsManager()
  {
    assertEngineIsInitialized();
    return *m_graphicsManager;
  }

  SceneManager& HotCoffeeEngine::getSceneManager()
  {
    assertEngineIsInitialized();
    return *m_sceneManager;
  }

  AssetManager& HotCoffeeEngine::getAssetManager()
  {
    assertEngineIsInitialized();
    return m_assetManager;
  }

  Time HotCoffeeEngine::getElapsedTime() const
  {
    assertEngineIsInitialized();
    return m_frameClock.getElapsedTime();
  }

  bool HotCoffeeEngine::isInitialized() const
  {
    return m_initialized;
  }

  void HotCoffeeEngine::addEventListener(IEventListener* listener)
  {
    if (!listener)
      throw InvalidArgumentException("Event listener pointer cannot be null.");
    m_eventListeners.push_back(listener);
  }

  void HotCoffeeEngine::removeEventListener(IEventListener* listener)
  {
    if (!listener)
      return;

    auto it = std::find(
      m_eventListeners.begin(),
      m_eventListeners.end(),
      listener
    );

    if (it != m_eventListeners.end())
      m_eventListeners.erase(it);
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

      m_sceneManager = SceneManagerFactory::create();

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

  void HotCoffeeEngine::run()
  {
    assertEngineIsInitialized();

    IWindow& window = m_windowManager->getWindow();
    m_frameClock.start();

    while (window.isOpen())
    {
      Optional<Event> eventOpt;
      while ((eventOpt = window.pollEvent()))
      {
        if (eventOpt->is<Event::Closed>())
        {
          m_frameClock.stop();
          window.destroy();
          return;
        }

        for (IEventListener* listener : m_eventListeners)
        {
          if (listener->onEvent(*eventOpt))
            break;
        }
      }

      m_sceneManager->update(m_frameClock.getElapsedTime());
      m_graphicsManager->beginFrame();
      m_sceneManager->draw();
      m_graphicsManager->endFrame(window);
      m_frameClock.restart();
    }
  }

  void HotCoffeeEngine::destroy()
  {
    m_frameClock.stop();
    m_assetManager.destroy();

    if (m_sceneManager)
    {
      m_sceneManager->destroy();
      m_sceneManager.reset();
    }

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

  void HotCoffeeEngine::assertEngineIsInitialized() const
  {
    if (!m_initialized)
    {
      throw RuntimeErrorException(
        "HotCoffeeEngine is not initialized. Call initialize() before using engine features."
      );
    }
  }
}
