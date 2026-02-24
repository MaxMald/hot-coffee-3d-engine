#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/window/hcWindowSettings.h"
#include "hc/plugins/hcPluginManagerSettings.h"

namespace hc
{
  struct HotCoffeeEngineSettings
  {
    WindowSettings windowSettings;
    PluginManagerSettings pluginManagerSettings;
  };
}
