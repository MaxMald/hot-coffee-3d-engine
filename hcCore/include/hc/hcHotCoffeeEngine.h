#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcHotCoffeeEngineSettings.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/inputManager/hcInputManager.h"
#include "hc/hcProcessResult.h"

namespace hc
{
  class IGameLoopListener;
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
    
    void addGameLoopListener(IGameLoopListener* listener);
    void removeGameLoopListener(IGameLoopListener* listener);

    ProcessResult initialize(const HotCoffeeEngineSettings& settings);
    void run(const String& sceneName);
    void run();
    void destroy();

  private:
    IGraphicsManager* m_graphicsManager;
    IWindowManager* m_windowManager;
    IAssetManager* m_assetManager;
    UniquePtr<SceneManager> m_sceneManager;
    PluginManager m_pluginManager;
    InputManager m_inputManager;
    Clock m_frameClock;
    Vector<IGameLoopListener*> m_eventListeners;
    bool m_initialized;

    void connectToPlugins(const PluginManagerSettings& settings);
    void assertEngineIsInitialized() const;
  };
}
