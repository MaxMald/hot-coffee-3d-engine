#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IPlugin;
  class IPluginSlot;
  class IPluginSlotFactory;

  /**
  * Manages the lifecycle and dependencies of plugins within the system.
  */
  class HC_CORE_EXPORT PluginManager : public NonCopyable
  {
  public:
    PluginManager();
    ~PluginManager();

    /**
     * Initializes the PluginManager.
     */
    void init();

    /**
    * Attempts to connect to the specified plug-in.
    *
    * @param _key The plug-in key.
    * @param _libraryName The library name of the plug-in.
    * @param _constructorFunctionName The name of the function that builds the
    * plug-in.
    * @param _destructorFunctionName The name of the function that destroys the
    * plug-in.
    *
    * @return
    */
    bool connectPlugin(
      const String& _key,
      const String& _libraryName,
      const String& _constructorFunctionName,
      const String& _destructorFunctionName
    );

    /**
    * Unload a plug-in.
    *
    * @param _key The plug-in key.
    *
    * @return
    */
    bool closePlugin(const String& _key);

    /**
    * Disconnect all plug-ins.
    */
    void closeAll();

    /**
    * Indicates if a plug-in exists in this plug-in manager.
    *
    * @param _key Plug-in key.
    *
    * @return True if the plug-in exists, otherwise returns false.
    */
    bool hasPlugin(const String& _key);

    /**
    * Get the plug-in with the given key. A null pointer will be returned if
    * the plug-in doesn't exists.
    *
    * @param _key Plug-in key.
    *
    * @return The pointer of the plug-in. This could return a null pointer if
    * the plug-in doesn't exists.
    */
    SharedPtr<IPlugin> getPlugin(const String& _key);

    /**
    * Add dependencies provided by all connected plug-ins to the given
    * container.
    */
    void addDependenciesFromPlugins(DependencyContainer& container);

    /**
    * @brief Gets the map of plugin slots.
    */
    const UnorderedMap<String, SharedPtr<IPluginSlot>>& getPluginSlots() const;

  private:
    SharedPtr<IPluginSlotFactory> m_pluginSlotFactory;
    UnorderedMap<String, SharedPtr<IPluginSlot>> m_pluginSlots;
  };
}
