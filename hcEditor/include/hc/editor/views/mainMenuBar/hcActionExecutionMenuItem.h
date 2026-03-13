#pragma once

#include "hc/editor/views/mainMenuBar/hcIMenuItem.h"

namespace hc::editor
{
  class IMenuItemAction;

  /**
   * @brief Menu item that executes a specific action when selected.
   */
  class ActionExectionMenuItem : public IMenuItem
  {
  public:
    /**
     * @brief Constructs an ActionExectionMenuItem with a name and action.
     * 
     * @param name The display name of the menu item.
     * @param action The action to execute when the menu item is selected.
     */
    ActionExectionMenuItem(const String& name, UniquePtr<IMenuItemAction> action);
    virtual ~ActionExectionMenuItem() = default;

    /**
     * @copydoc IMenuItem::draw
     */
    void draw() override;

  private:
    String m_name;
    UniquePtr<IMenuItemAction> m_action;
  };
}
