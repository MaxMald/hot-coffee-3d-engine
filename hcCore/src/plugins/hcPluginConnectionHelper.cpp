#include "hc/plugins/hcPluginConnectionHelper.h"
#include "hc/plugins/hcPluginManager.h"
#include "hc/plugins/hcPluginManagerSettings.h"
#include "hc/plugins/hcPluginStandardKeys.h"

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

      connectAssetManagerPlugin(pluginManager);
    }

    void connectWindowSfmlPlugin(PluginManager& pluginManager)
    {
      pluginManager.connectPlugin(
        pluginStandardKeys::WindowPlugin,
        String(HC_DYN_LIB_PREFIX) + "hcWindowSfml" + String(HC_DYN_LIB_SUFIX),
        "createWindowSfmlPlugin",
        "destroyWindowSfmlPlugin"
      );
    }

    void connectGraphicsOpenGLPlugin(PluginManager& pluginManager)
    {
      pluginManager.connectPlugin(
        pluginStandardKeys::GraphicsPlugin,
        String(HC_DYN_LIB_PREFIX) + "hcGraphicsOpenGL" + String(HC_DYN_LIB_SUFIX),
        "createGraphicsOpenGLPlugin",
        "destroyGraphicsOpenGLPlugin"
      );
    }

    void connectAssetManagerPlugin(PluginManager& pluginManager)
    {
      pluginManager.connectPlugin(
        pluginStandardKeys::AssetManagerPlugin,
        String(HC_DYN_LIB_PREFIX) + "hcAssetManager" + String(HC_DYN_LIB_SUFIX),
        "createAssetManagerPlugin",
        "destroyAssetManagerPlugin"
      );
    }
  }
}
