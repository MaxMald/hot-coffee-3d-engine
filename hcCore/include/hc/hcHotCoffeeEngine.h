#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcHotCoffeeEngineSettings.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/inputManager/hcInputManager.h"
#include "hc/hcProcessResult.h"

namespace hc
{
  class IEventListener;
  class SceneManager;
  class IWindowManager;
  class IGraphicsManager;
  class IAssetManager;

  class HC_CORE_EXPORT HotCoffeeEngine
  {
  public:
    HotCoffeeEngine();
    ~HotCoffeeEngine();

    const PluginManager& getPluginManager() const;
    IWindowManager& getWindowManager();
    IGraphicsManager& getGraphicsManager();
    SceneManager& getSceneManager();
    IAssetManager& getAssetManager();
    InputManager& getInputManager();
    Time getElapsedTime() const;
    bool isInitialized() const;
    
    void addEventListener(IEventListener* listener);
    void removeEventListener(IEventListener* listener);

    ProcessResult initialize(const HotCoffeeEngineSettings& settings);
    void run(const String& sceneName);
    void destroy();

  private:
    UniquePtr<IGraphicsManager> m_graphicsManager;
    IWindowManager* m_windowManager;
    UniquePtr<SceneManager> m_sceneManager;
    IAssetManager* m_assetManager;
    PluginManager m_pluginManager;
    InputManager m_inputManager;
    Clock m_frameClock;
    Vector<IEventListener*> m_eventListeners;
    bool m_initialized;

    void connectToPlugins(const PluginManagerSettings& settings);
    void assertEngineIsInitialized() const;
  };
}
