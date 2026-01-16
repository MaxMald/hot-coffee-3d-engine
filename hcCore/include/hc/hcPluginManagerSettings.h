#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
  * @brief Configuration settings for the plugin manager.
  *
  * This struct defines the types plugins that can be use by the engine. It is
  * intended to be used as a simple configuration holder to specify which plugins
  * should be loaded.
  */
  struct PluginManagerSettings
  {
  public:

    /**
     * @brief Supported window plugin types.
     */
    enum class windowPluginType : UInt8
    {
      SFML
    };

    /**
     * @brief Supported graphics plugin types.
     */
    enum class graphicsPluginType : UInt8
    {
      OPENGL
    };

    /**
     * @brief The selected window plugin type.
     */
    windowPluginType windowType = windowPluginType::SFML;

    /**
     * @brief The selected graphics plugin type.
     */
    graphicsPluginType graphicsType = graphicsPluginType::OPENGL;
  };
}
