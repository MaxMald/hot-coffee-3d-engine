#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc
{
  class PluginManager;
  class IPluginSlot;
}

namespace hc::editor
{
   /**
   * @brief Plugin Manager window view for the Hot Coffee Editor.
   *
   * Implements a window that displays and manages plugins within the editor.
   */
  class  PluginManagerWindow : public AWindowView
  {
  public:
    /**
     * @brief Constructs the Plugin Manager window.
     * 
     * @param pluginManager Reference to the PluginManager instance.
     */
    PluginManagerWindow(const PluginManager& pluginManager);

    /**
     * @brief Virtual destructor.
     */
    virtual ~PluginManagerWindow();

  protected:
    const PluginManager& m_pluginManager;

    /**
     * @brief Draws the contents of the Plugin Manager window.
     */
    void onDraw() override;

    /**
     * @brief Draws information for a specific plugin slot.
     *
     * @param plugin Reference to the plugin slot to display information for.
     */
    void drawPluginSlotInfo(const IPluginSlot& plugin);
  };
}
