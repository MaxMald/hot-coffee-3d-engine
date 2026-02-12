#pragma once

#include "hc/editor/hcAWindowView.h"

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
    PluginManagerWindow(const PluginManager& pluginManager);
    virtual ~PluginManagerWindow();

  protected:
    const PluginManager& m_pluginManager;

    void onDraw() override;
    void drawPluginSlotInfo(const IPluginSlot& plugin);
  };
}
