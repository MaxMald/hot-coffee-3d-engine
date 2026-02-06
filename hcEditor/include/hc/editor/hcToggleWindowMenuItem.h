#pragma once

#include "hc/editor/hcIMenuItem.h"

namespace hc::editor
{
  class AWindowView;

  /**
   * @brief Menu item that toggles the visibility of a window view in the editor.
   */
  class ToggleWindowMenuItem : public IMenuItem
  {
  public:
    /**
     * @brief Constructs a ToggleWindowMenuItem for the given window view.
     *
     * @param windowView Reference to the window view to be toggled.
     */
    ToggleWindowMenuItem(AWindowView& windowView);
    virtual ~ToggleWindowMenuItem() = default;

    /**
     * @copydoc IMenuItem::draw
     */
    void draw() override;

  private:
    AWindowView& m_windowView;
  };
}
