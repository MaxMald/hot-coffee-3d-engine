#include "hc/editor/hcActionExecutionMenuItem.h"
#include "hc/editor/hcIMenuItemAction.h"
#include "imgui.h"

namespace hc::editor
{
  ActionExectionMenuItem::ActionExectionMenuItem(
    const String& name,
    UniquePtr<IMenuItemAction> action
  ) : m_action(std::move(action)),
    m_name(name)
  {
  }

  void ActionExectionMenuItem::draw()
  {
    if (ImGui::MenuItem(m_name.c_str()))
    {
      if (m_action)
        m_action->execute();
    }
  }
}
