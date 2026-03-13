#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Interface for a menu item in the editor.
   *
   * Implement this interface to define custom menu items that can be drawn
   * within the editor's UI.
   */
  class IMenuItem
  {
  public:
    virtual ~IMenuItem() = default;

    /**
     * @brief Draws the menu item in the editor UI.
     */
    virtual void draw() = 0;

  protected:
    IMenuItem() = default;
  };
}
