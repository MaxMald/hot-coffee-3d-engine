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

    /**
     * @brief Destroys the view and releases resources.
     */
    virtual void destroy() = 0;

  protected:
    IView() = default;
  };
}
