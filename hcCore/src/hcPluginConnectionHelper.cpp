#include "hc/hcPluginConnectionHelper.h"
#include "hc/hcPluginManager.h"

namespace hc
{
  namespace pluginConnectionHelper
  {
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
