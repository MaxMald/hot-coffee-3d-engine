#pragma once

#include <hc/hcIDependencyResolvable.h>
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
  class  PluginManagerWindow : public AWindowView, public IDependencyResolvable
  {
  public:
    PluginManagerWindow();
    virtual ~PluginManagerWindow();

    void resolveDependencies(DependencyContainer& container) override;

  protected:
    const PluginManager* m_pluginManager;

    void onDraw() override;
    void drawPluginSlotInfo(const IPluginSlot& plugin);
  };
}
