#include "hc/editor/views/mainMenuBar/hcMenu.h"
#include "imgui.h"

namespace hc::editor
{
  Menu::Menu(const String& name) :
    IMenuItem(),
    m_name(name)
  {
  }

  void Menu::addItem(UniquePtr<IMenuItem> menuElement)
  {
    m_elements.push_back(std::move(menuElement));
  }

  void Menu::draw()
  {
    if (ImGui::BeginMenu(m_name.c_str()))
    {
      for (const auto& element : m_elements)
        element->draw();
      ImGui::EndMenu();
    }
  }
}
