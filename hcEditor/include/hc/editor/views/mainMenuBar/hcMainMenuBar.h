#pragma once

#include "hc/editor/views/hcABaseView.h"

namespace hc::editor
{
  class Menu;

  /**
   * @brief Represents the main menu bar in the editor UI.
   *
   * The MainMenuBar class manages a collection of top-level menus and provides
   * functionality to add menus and render the main menu bar in the editor
   * interface.
   */
  class MainMenuBar : public ABaseView
  {
  public:
    MainMenuBar();
    ~MainMenuBar();

    /**
     * @brief Adds a top-level menu to the main menu bar.
     * 
     * @param menu The menu to add.
     */
    void addMenu(UniquePtr<Menu> menu);

    /**
     * @brief Draws the main menu bar and all its menus in the editor UI.
     */
    void draw() override;

  private:
    Vector<UniquePtr<Menu>> m_menus;
  };
}
