#include "hc/hcPluginConnectionHelper.h"
#include "hc/hcPluginManager.h"
#include "hc/hcPluginManagerSettings.h"
#include "hc/hcPluginStandardKeys.h"

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

      connectAssetLoadersPlugin(pluginManager);
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

    void connectAssetLoadersPlugin(PluginManager& pluginManager)
    {
      pluginManager.connectPlugin(
        pluginStandardKeys::AssetLoadersPlugin,
        String(HC_DYN_LIB_PREFIX) + "hcAssetLoaders" + String(HC_DYN_LIB_SUFIX),
        "createAssetLoadersPlugin",
        "destroyAssetLoadersPlugin"
      );
    }
  }
}
