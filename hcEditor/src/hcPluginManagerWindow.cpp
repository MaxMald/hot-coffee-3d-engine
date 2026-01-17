#include "hc/editor/hcPluginManagerWindow.h"

#include "imgui.h"
#include <hc/hcPluginManager.h>
#include <hc/hcIPluginSlot.h>
#include <hc/hcIPlugin.h>

namespace hc::editor
{
  PluginManagerWindow::PluginManagerWindow(const PluginManager& pluginManager) :
    AWindowView("Plugin Manager"),
    m_pluginManager(pluginManager)
  {
  }

  PluginManagerWindow::~PluginManagerWindow()
  {
  }

  void PluginManagerWindow::onDraw()
  {
    UnorderedMap<String, SharedPtr<IPluginSlot>> pluginSlots =
      m_pluginManager.getPluginSlots();

    if (ImGui::TreeNode("Plugins"))
    {
      for (const auto& [key, pluginSlot] : pluginSlots)
      {
        if (pluginSlot == nullptr)
          continue;

        if (ImGui::TreeNode(key.c_str()))
        {
          drawPluginSlotInfo(*pluginSlot);
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }

  void PluginManagerWindow::drawPluginSlotInfo(
    const IPluginSlot& plugin
  )
  {
    String key = plugin.getKey();
    ImGui::Text("Plugin Key: %s", plugin.getKey().c_str());
    ImGui::Text("Library Name: %s", plugin.getLibraryName().c_str());
    ImGui::Text("Constructor Function: %s", plugin.getConstructorFunctionName().c_str());
    ImGui::Text("Destructor Function: %s", plugin.getDestructorFunctionName().c_str());
  }
}
