#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcPluginManager.h"
#include "hc/hcHotCoffeeEngineSettings.h"

namespace hc
{
  class IWindowManager;
  class IGraphicsManager;
  class AssetManager;

  class HC_CORE_EXPORT HotCoffeeEngine : public AModule<HotCoffeeEngine>
  {
  public:
    static void Initialize(const HotCoffeeEngineSettings& settings);
    static IGraphicsManager& GetGraphicsManager();
    static AssetManager& GetAssetManager();
    static IWindowManager& GetWindowManager();

    HotCoffeeEngine();
    ~HotCoffeeEngine() override;

    const PluginManager& getPluginManager() const;
    IWindowManager& getWindowManager();
    IGraphicsManager& getGraphicsManager();
    AssetManager& getAssetManager();

  private:
    SharedPtr<IWindowManager> m_windowManager;
    SharedPtr<IGraphicsManager> m_graphicsManager;
    SharedPtr<AssetManager> m_assetManager;
    PluginManager m_pluginManager;
    DependencyContainer m_dependencyContainer;
    bool m_initialized;

    void onPrepare() override;
    void onShutdown() override;

    void initialize(const HotCoffeeEngineSettings& settings);
    void connectToPlugins(const PluginManagerSettings& settings);
    void registerDependencies();
    void resolveDependencies();
  };
}
