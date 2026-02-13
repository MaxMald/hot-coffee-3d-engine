#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcPluginManager.h"
#include "hc/hcHotCoffeeEngineSettings.h"
#include "hc/hcAssetManager.h"
#include "hc/hcProcessResult.h"

namespace hc
{
  class SceneManager;
  class IWindowManager;
  class IGraphicsManager;

  class HC_CORE_EXPORT HotCoffeeEngine
  {
  public:
    HotCoffeeEngine();
    ~HotCoffeeEngine();

    const PluginManager& getPluginManager() const;
    IWindowManager& getWindowManager();
    IGraphicsManager& getGraphicsManager();
    SceneManager& getSceneManager();
    AssetManager& getAssetManager();
    bool isInitialized() const;

    ProcessResult initialize(const HotCoffeeEngineSettings& settings);
    void destroy();

  private:
    UniquePtr<IGraphicsManager> m_graphicsManager;
    UniquePtr<IWindowManager> m_windowManager;
    UniquePtr<SceneManager> m_sceneManager;
    AssetManager m_assetManager;
    PluginManager m_pluginManager;
    bool m_initialized;

    void connectToPlugins(const PluginManagerSettings& settings);
  };
}
