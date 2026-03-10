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
      static_cast<IWindowManagerPlugin*>(&plugin);

    return windowManagerPlugin->getWindowManager();
  }
}
