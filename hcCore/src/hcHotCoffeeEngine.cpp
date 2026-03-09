#include "hc/hcHotCoffeeEngine.h"

#include <algorithm>

#include "hc/serialization/hcJsonSerializer.h"
#include "hc/plugins/hcPluginConnectionHelper.h"
#include "hc/window/hcIWindowManager.h"
#include "hc/window/hcIWindow.h"
#include "hc/graphics/hcIGraphicsManager.h"
#include "hc/scene/hcSceneManager.h"
#include "hc/hcIEventListener.h"
#include "hc/assets/hcAssetManagerPluginAccessor.h"
#include "hc/assets/hcIAssetManager.h"

#include "hc/graphics/hcGraphicsManagerFactory.h"
#include "hc/window/hcWindowManagerPluginAccessor.h"
#include "hc/scene/hcSceneManagerFactory.h"

namespace hc
{
  HotCoffeeEngine::HotCoffeeEngine() :
    m_graphicsManager(nullptr),
    m_windowManager(nullptr),
    m_sceneManager(nullptr),
    m_assetManager(nullptr),
    m_pluginManager(),
    m_inputManager(),
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

  IAssetManager& HotCoffeeEngine::getAssetManager()
  {
    assertEngineIsInitialized();
    return *m_assetManager;
  }

  InputManager& HotCoffeeEngine::getInputManager()
  {
    assertEngineIsInitialized();
    return m_inputManager;
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
      m_assetManager = &(AssetManagerPluginAccessor::GetAssetManager(m_pluginManager));
      m_windowManager = &(WindowManagerPluginAccessor::GetWindowManager(m_pluginManager));
      m_windowManager->createWindow(settings.windowSettings);

      m_graphicsManager = graphicsManagerFactory::Create(
        m_pluginManager,
        m_windowManager->getWindow(),
        *m_assetManager
      );
      m_graphicsManager->initialize();

      m_inputManager.initialize(*this);
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

  void HotCoffeeEngine::run(const String& sceneName)
  {
    assertEngineIsInitialized();

    if (!m_sceneManager->setActiveScene(sceneName))
    {
      throw RuntimeErrorException(
        "Failed to set active scene. Scene with name '" + sceneName + "' not found."
      );
    }

    IWindow& window = m_windowManager->getWindow();
    m_frameClock.start();

    while (window.isOpen())
    {
      m_inputManager.prepareForEventPolling();

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
    m_inputManager.destroy(*this);

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

    m_pluginManager.closeAll();
    m_assetManager = nullptr;
    m_windowManager = nullptr;

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
