#include "hc/editor/hcHotCoffeeEngineSettingsFactory.h"

namespace hc::editor
{
  namespace hotCoffeeEngineSettingsFactory
  {
    HotCoffeeEngineSettings createDefault()
    {
      HotCoffeeEngineSettings settings;

      settings.windowSettings.width = 1920;
      settings.windowSettings.height = 1080;
      settings.windowSettings.title = "HotCoffee Editor";

      settings.pluginManagerSettings.windowType =
        PluginManagerSettings::windowPluginType::SFML;
      settings.pluginManagerSettings.graphicsType =
        PluginManagerSettings::graphicsPluginType::OPENGL;

      return settings;
    }
  }
}
