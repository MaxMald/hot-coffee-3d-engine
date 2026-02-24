#pragma once

#include "hc/editor/views/mainMenuBar/hcIMenuItem.h"

namespace hc::editor
{
  /**
   * @brief Represents a menu that can contain multiple menu items in the editor.
   *
   * The Menu class implements IMenuItem and allows grouping of other menu items,
   * providing functionality to add items and render the menu in the editor UI.
   */
  class Menu : public IMenuItem
  {
  public:
    /**
     * @brief Constructs a Menu with the specified name.
     * 
     * @param name The display name of the menu.
     */
    Menu(const String& name);
    virtual ~Menu() = default;

    /**
     * @brief Adds a menu item to this menu.
     * 
     * @param menuElement The menu item to add.
     */
    void addItem(UniquePtr<IMenuItem> menuElement);

    /**
     * @brief Draws the menu and its contained items in the editor UI.
     */
    void draw() override;

  private:
    String m_name;
    Vector<UniquePtr<IMenuItem>> m_elements;
  };
}
