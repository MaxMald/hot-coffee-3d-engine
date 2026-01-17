#pragma once

#include "hc/editor/hcIView.h"

namespace hc::editor
{
  /**
   * @brief Abstract base class for ImGui-based window views.
   *
   * Provides a common interface and state for ImGui windows, including
   * open/close management.
   */
  class AWindowView : public IView
  {
  public:
    /**
     * @brief Constructs a WindowView with the given name.
     * 
     * @param name The window's display name.
     * @param isOpen Initial open state of the window.
     */
    AWindowView(const String& name, bool isOpen = false);

    /**
     * @brief Virtual destructor.
     */
    virtual ~AWindowView();

    /**
     * @brief Draws the window contents.
     */
    virtual void draw() override;

    /**
     * @brief Returns whether the window is currently open.
     */
    bool isOpen() const;

    /**
     * @brief Sets the open state of the window.
     * 
     * @param open True to show the window, false to hide.
     */
    void setOpen(bool open);

    /**
     * @brief Provides access to the open state flag.
     *
     * @return Reference to the open state flag.
     */
    bool& getOpenFlagReference();

  protected:
    String m_windowName;
    bool m_isOpen = true;

    /**
     * @brief Pure virtual method for drawing the window's specific contents.
     *
     * Derived classes must implement this to define their UI.
     */
    virtual void onDraw() = 0;
  };
}
