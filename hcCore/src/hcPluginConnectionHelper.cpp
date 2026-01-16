#include "hc/hcPluginConnectionHelper.h"
#include "hc/hcPluginManager.h"
#include "hc/hcPluginManagerSettings.h"

namespace hc
{
  namespace pluginConnectionHelper
  {
    void connectToPluginsFromSettings(
      PluginManager& pluginManager,
      const PluginManagerSettings& settings
    )
    {
      if (settings.windowType == PluginManagerSettings::windowPluginType::SFML)
        connectWindowSfmlPlugin(pluginManager);

      // Future plugin types can be connected here.
    }

    void connectWindowSfmlPlugin(PluginManager& pluginManager)
    {
      pluginManager.connectPlugin(
        "WindowPlugin",
        "hcWindowSfml" + String(HC_DYN_LIB_SUFIX),
        "createWindowSfmlPlugin",
        "destroyWindowSfmlPlugin"
      );
    }
  }
}
