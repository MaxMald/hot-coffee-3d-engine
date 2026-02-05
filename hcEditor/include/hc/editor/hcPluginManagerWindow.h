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
    PluginManagerWindow();
    virtual ~PluginManagerWindow();

  protected:
    void onDraw() override;
    void drawPluginSlotInfo(const IPluginSlot& plugin);
  };
}
