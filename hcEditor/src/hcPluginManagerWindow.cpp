#include "hc/editor/hcPluginManagerWindow.h"

#include "hc/editor/hcEditorViewsManager.h"
#include "imgui.h"

namespace hc::editor
{
  PluginManagerWindow::PluginManagerWindow() : 
    AWindowView("Plugin Manager")
  {
  }

  PluginManagerWindow::~PluginManagerWindow()
  {
  }

  void PluginManagerWindow::onDraw()
  {
    UnorderedMap<String, SharedPtr<IPluginSlot>> pluginSlots =
      HotCoffeeEngine::Instance().getPluginManager().getPluginSlots();

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
