#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class Menu;
  class IMenuItem;

  /**
   * @brief Builder class for constructing hierarchical menus.
   */
  class MenuBuilder
  {
  public:
    MenuBuilder();
    ~MenuBuilder();

    /**
     * @brief Begins a new menu with the given name and pushes it onto the stack.
     * 
     * @param name The display name of the new menu.
     * 
     * @return Reference to this builder for chaining.
     */
    MenuBuilder& beginMenu(const String& name);

    /**
     * @brief Ends the current menu, popping it from the stack and adding it to
     * its parent.
     * 
     * @return Reference to this builder for chaining.
     * 
     * @throws RuntimeErrorException if the menu stack is empty.
     */
    MenuBuilder& endMenu();

    /**
     * @brief Adds a menu item to the current menu on the stack.
     * 
     * @param menuItem The menu item to add.
     * 
     * @return Reference to this builder for chaining.
     * @throws RuntimeErrorException if the menu stack is empty.
     */
    MenuBuilder& addMenuItem(UniquePtr<IMenuItem> menuItem);

    /**
     * @brief Finalizes and returns the constructed menu.
     *
     * @return The root menu as a UniquePtr.
     * @throws RuntimeErrorException if there are unclosed menus in the stack.
     */
    UniquePtr<Menu> build();

  private:
    Vector<UniquePtr<Menu>> m_menuStack;

    void assertMenuStackNotEmpty() const;
  };
}
