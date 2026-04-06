#pragma once

#include "hc/editor/views/hcIView.h"

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
  class MainMenuBar : public IView
  {
  public:
    MainMenuBar();
    ~MainMenuBar();

    /**
     * @copydoc IView::update
     */
    void update(const Time& elapsedTime) override;

    /**
     * @brief Draws the main menu bar and all its menus in the editor UI.
     */
    void draw() override;

    /**
     * @copydoc IView::destroy
     */
    void destroy() override;

    /**
     * @brief Adds a top-level menu to the main menu bar.
     * 
     * @param menu The menu to add.
     */
    void addMenu(UniquePtr<Menu> menu);

  private:
    Vector<UniquePtr<Menu>> m_menus;
  };
}
