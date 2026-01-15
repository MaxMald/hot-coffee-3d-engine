#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcPluginManager.h"
#include "hc/hcDependencyContainer.h";

namespace hc
{
  class HC_CORE_EXPORT HotCoffeeEngine
  {
  public:
    static HotCoffeeEngine& Instance();
    static void Prepare();
    static void Shutdown();

    const PluginManager& getPluginManager() const;

    void start();

  private:
    static HotCoffeeEngine* _Instance;

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
