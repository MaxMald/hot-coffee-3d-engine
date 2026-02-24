#include "hc/editor/views/mainMenuBar/hcToggleWindowMenuItem.h"
#include "hc/editor/views/windows/hcAWindowView.h"
#include "imgui.h"

namespace hc::editor
{
  ToggleWindowMenuItem::ToggleWindowMenuItem(AWindowView& windowView)
    : m_windowView(windowView)
  {
  }

  void ToggleWindowMenuItem::draw()
  {
    if (ImGui::MenuItem(m_windowView.getWindowName().c_str()))
    {
      if (m_windowView.isOpen())
        m_windowView.setOpen(false);
      else
        m_windowView.setOpen(true);
    }
  }
}
