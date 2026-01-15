#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcPluginManager.h"
#include "hc/hcDependencyContainer.h";
#include "hc/hcHotCoffeeEngineSettings.h"

namespace hc
{
  class WindowManager;

  class HC_CORE_EXPORT HotCoffeeEngine
  {
  public:
    static HotCoffeeEngine& Instance();
    static void Prepare();
    static void Shutdown();

    const PluginManager& getPluginManager() const;
    WindowManager& getWindowManager();

    void start(const HotCoffeeEngineSettings& settings);

  private:
    static HotCoffeeEngine* _Instance;

    SharedPtr<WindowManager> m_windowManager;

    bool m_started;
    PluginManager m_pluginManager;
    DependencyContainer m_dependencyContainer;

    HotCoffeeEngine();
    ~HotCoffeeEngine();

    void onPrepare();
    void onShutdown();

    void prepareAndResolveDependencyContainer();
  };
}
