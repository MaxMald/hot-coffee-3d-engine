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

      if (settings.graphicsType == PluginManagerSettings::graphicsPluginType::OPENGL)
        connectGraphicsOpenGLPlugin(pluginManager);
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

    void connectGraphicsOpenGLPlugin(PluginManager& pluginManager)
    {
      pluginManager.connectPlugin(
        "GraphicsPlugin",
        "hcGraphicsOpenGL" + String(HC_DYN_LIB_SUFIX),
        "createGraphicsOpenGLPlugin",
        "destroyGraphicsOpenGLPlugin"
      );
    }
  }
}
