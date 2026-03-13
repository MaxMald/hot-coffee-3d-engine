#include "hc/editor/views/mainMenuBar/hcMainMenuBar.h"
#include "hc/editor/views/mainMenuBar/hcMenu.h"
#include "imgui.h"

namespace hc::editor
{
  MainMenuBar::MainMenuBar()
  {
  }

  MainMenuBar::~MainMenuBar()
  {
  }

  void MainMenuBar::addMenu(UniquePtr<Menu> menu)
  {
    m_menus.push_back(std::move(menu));
  }

  void MainMenuBar::draw()
  {
    if (ImGui::BeginMainMenuBar())
    {
      for (const auto& menu : m_menus)
        menu->draw();
      ImGui::EndMainMenuBar();
    }
  }
}
