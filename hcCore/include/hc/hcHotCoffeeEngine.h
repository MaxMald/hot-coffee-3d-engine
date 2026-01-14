#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class HC_CORE_EXPORT HotCoffeeEngine
  {
  public:
    static HotCoffeeEngine& Instance();
    static void Prepare();
    static void Shutdown();

    void start();

  private:
    static HotCoffeeEngine* _Instance;

    void onPrepare();
    void onShutdown();

    void prepareAndResolveDependencyContainer();
    void resolveDependencies();

    HotCoffeeEngine();
    ~HotCoffeeEngine();
  };
}
