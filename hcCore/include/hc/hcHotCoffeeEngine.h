#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcPluginManager.h"

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

    void onPrepare();
    void onShutdown();

    void prepareAndResolveDependencyContainer();

    HotCoffeeEngine();
    ~HotCoffeeEngine();
  };
}
