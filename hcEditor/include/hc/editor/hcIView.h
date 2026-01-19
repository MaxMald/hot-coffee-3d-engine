#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Interface representing a view component in the editor.
   */
  class IView
  {
  public:
    virtual ~IView() = default;

    /**
     * @brief Draws the view.
     */
    virtual void draw() = 0;
  };
}
