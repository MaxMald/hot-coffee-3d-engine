#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Interface for actions that can be executed by a menu item.
   *
   * Implement this interface to define custom actions that are triggered when a
   * menu item is selected in the editor.
   */
  class IMenuItemAction
  {
  public:
    virtual ~IMenuItemAction() = default;

    /**
     * @brief Executes the action associated with the menu item.
     */
    virtual void execute() = 0;

  protected:
    IMenuItemAction() = default;
  };
}
