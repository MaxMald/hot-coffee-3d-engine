#include "hc/window/hcWindowManagerFactory.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/plugins/hcPluginStandardKeys.h"
#include "hc/plugins/hcIWindowManagerPlugin.h"
#include "hc/window/hcIWindowManager.h"

namespace hc
{
  namespace windowManagerFactory
  {
    UniquePtr<IWindowManager> Create(
      PluginManager& pluginManager
    )
    {
      SharedPtr<IPlugin> plugin = pluginManager.getPlugin(
        pluginStandardKeys::WindowPlugin
      );

      if (!plugin)
      {
        throw RuntimeErrorException(
          "Window plugin not found. Make sure the window plugin is correctly connected."
        );
      }

      SharedPtr<IWindowManagerPlugin> windowManagerPlugin =
        std::dynamic_pointer_cast<IWindowManagerPlugin>(plugin);

      if (!windowManagerPlugin)
      {
        throw RuntimeErrorException(
          "The connected window manager plugin does not implement the IWindowManagerPlugin interface."
        );
      }

      UniquePtr<IWindowManager> windowManager =
        windowManagerPlugin->createWindowManager();

      if (!windowManager)
      {
        throw RuntimeErrorException(
          "Failed to create window manager from plugin. Make sure the window plugin's createWindowManager function is correctly implemented and returns a valid IWindowManager instance."
        );
      }

      return windowManager;
    }
  }
}
