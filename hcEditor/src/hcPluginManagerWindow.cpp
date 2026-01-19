#include "hc/editor/hcPluginManagerWindow.h"

#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcPluginManager.h>
#include <hc/hcIPluginSlot.h>
#include <hc/hcIPlugin.h>
#include "hc/editor/hcEditorViewsManager.h"
#include "imgui.h"

namespace hc::editor
{
  PluginManagerWindow::PluginManagerWindow() :
    AWindowView("Plugin Manager"),
    m_pluginManager(nullptr)
  {
  }

  PluginManagerWindow::~PluginManagerWindow()
  {
  }

  void PluginManagerWindow::resolveDependencies(DependencyContainer& container)
  {
    SharedPtr<EditorViewsManager> viewsManager = container.resolve<EditorViewsManager>();
    viewsManager->registerView(this);

    m_pluginManager = &(HotCoffeeEngine::Instance().getPluginManager());
  }

  void PluginManagerWindow::onDraw()
  {
    UnorderedMap<String, SharedPtr<IPluginSlot>> pluginSlots =
      m_pluginManager->getPluginSlots();

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
