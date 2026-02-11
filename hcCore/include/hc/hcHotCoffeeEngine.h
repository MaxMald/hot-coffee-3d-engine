#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcPluginManager.h"
#include "hc/hcHotCoffeeEngineSettings.h"
#include "hc/hcSceneManager.h"
#include "hc/hcAssetManager.h"

namespace hc
{
  class IWindowManager;
  class IGraphicsManager;

  class HC_CORE_EXPORT HotCoffeeEngine : public AModule<HotCoffeeEngine>
  {
  public:
    static void Initialize(const HotCoffeeEngineSettings& settings);
    static IGraphicsManager& GetGraphicsManager();
    static SceneManager& GetSceneManager();
    static AssetManager& GetAssetManager();
    static IWindowManager& GetWindowManager();

    HotCoffeeEngine();
    ~HotCoffeeEngine() override;

    const PluginManager& getPluginManager() const;
    IWindowManager& getWindowManager();
    IGraphicsManager& getGraphicsManager();
    SceneManager& getSceneManager();
    AssetManager& getAssetManager();
    bool isInitialized() const;

  private:
    UniquePtr<IGraphicsManager> m_graphicsManager;
    UniquePtr<IWindowManager> m_windowManager;
    SceneManager m_sceneManager;
    AssetManager m_assetManager;
    PluginManager m_pluginManager;
    bool m_initialized;

    void onPrepare() override;
    void onShutdown() override;

    void initialize(const HotCoffeeEngineSettings& settings);
    void connectToPlugins(const PluginManagerSettings& settings);
    void destroy();
  };
}
