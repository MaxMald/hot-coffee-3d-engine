#include "hc/window/hcWindowManagerPluginAccessor.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/plugins/hcPluginStandardKeys.h"
#include "hc/plugins/hcIWindowManagerPlugin.h"
#include "hc/window/hcIWindowManager.h"

namespace hc
{
  IWindowManager* WindowManagerPluginAccessor::GetWindowManager(
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
      std::reinterpret_pointer_cast<IWindowManagerPlugin>(plugin);

    if (!windowManagerPlugin)
    {
      throw RuntimeErrorException(
        "The connected window manager plugin does not implement the IWindowManagerPlugin interface."
      );
    }

    return windowManagerPlugin->getWindowManager();
  }
}
