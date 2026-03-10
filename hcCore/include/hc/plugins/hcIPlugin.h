#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
  * Abstract interface for dynamically loadable plugin modules.
  *
  * This interface defines the contract that all plugin DLLs must implement
  * to integrate with the PluginManager. Concrete implementations provide
  * specific functionality (e.g., asset management, rendering) while the
  * PluginManager handles the lifecycle of loading, connecting, and
  * unloading the plugin modules.
  *
  * Plugin DLLs must export factory functions to create and destroy instances
  * of their IPlugin implementation.
  */
  class HC_CORE_EXPORT IPlugin : public NonCopyable
  {
  public:
    virtual ~IPlugin() = default;

    /**
    * Lifecycle hook called immediately after the plugin is loaded and
    * connected by the PluginManager.
    *
    * Use this method to initialize plugin resources, register services, or
    * perform any setup required before the plugin becomes operational.
    */
    virtual void onConnect() = 0;

    /**
    * Called when the plug-in is about to be closed.
    */
    virtual void onClose() = 0;

  protected:
    IPlugin() = default;
  };
}
