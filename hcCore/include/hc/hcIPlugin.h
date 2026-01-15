#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"

namespace hc
{
  class DependencyContainer;

  /**
  * Provides connection from a PluginManager to a DLL services.
  *
  * The IPlugin is in charge of initialize and provides access to the library
  * services, also closes and free the memory when the plug-in is been closed.
  */
  class HC_CORE_EXPORT IPlugin : public NonCopyable
  {
  public:
    virtual ~IPlugin() = default;

    /**
    * Called when the plug-in has just been connected.
    */
    virtual void onConnect() = 0;

    /**
    * Called when the plug-in is about to been closed.
    */
    virtual void onClose() = 0;

    /**
    * Get the wrapped data of this plug-in.
    *
    * @return Wrapped data.
    */
    virtual void* getData() = 0;

    /**
    * Add dependencies provided by this plug-in to the given container.
    */
    virtual void addDependencies(DependencyContainer& container) = 0;
  };
}
