#include "hc/window/hcWindowManagerPluginAccessor.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/plugins/hcPluginStandardKeys.h"
#include "hc/plugins/hcIWindowManagerPlugin.h"
#include "hc/window/hcIWindowManager.h"

namespace hc
{
  IWindowManager& WindowManagerPluginAccessor::GetWindowManager(
    PluginManager& pluginManager
  )
  {
    IPlugin& plugin = pluginManager.getPlugin(
      pluginStandardKeys::WindowPlugin
    );

    IWindowManagerPlugin* windowManagerPlugin =
      reinterpret_cast<IWindowManagerPlugin*>(&plugin);

    if (!windowManagerPlugin)
    {
      throw RuntimeErrorException(
        "The connected window manager plugin does not implement the IWindowManagerPlugin interface."
      );
    }

    return windowManagerPlugin->getWindowManager();
  }
}
