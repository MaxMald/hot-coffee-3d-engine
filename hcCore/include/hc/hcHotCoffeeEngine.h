#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcPluginManager.h"
#include "hc/hcHotCoffeeEngineSettings.h"

namespace hc
{
  class IWindowManager;
  class IGraphicsManager;
  class SceneManager;
  class AssetManager;

  class HC_CORE_EXPORT HotCoffeeEngine : public AModule<HotCoffeeEngine>
  {
  public:
    HotCoffeeEngine();
    ~HotCoffeeEngine() override;

    const PluginManager& getPluginManager() const;
    IWindowManager& getWindowManager();
    IGraphicsManager& getGraphicsManager();
    SceneManager& getSceneManager();
    AssetManager& getAssetManager();

    void init(const HotCoffeeEngineSettings& settings);

  protected:
    void onPrepare() override;
    void onShutdown() override;

  private:
    SharedPtr<IWindowManager> m_windowManager;
    SharedPtr<IGraphicsManager> m_graphicsManager;
    SharedPtr<SceneManager> m_sceneManager;
    SharedPtr<AssetManager> m_assetManager;
    PluginManager m_pluginManager;
    DependencyContainer m_dependencyContainer;
    bool m_started;

    void connectToPlugins(const PluginManagerSettings& settings);
    void registerDependencies();
    void resolveDependencies();
  };
}
