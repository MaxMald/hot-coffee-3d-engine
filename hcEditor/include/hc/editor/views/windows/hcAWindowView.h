#pragma once

#include "hc/editor/views/hcABaseView.h"

namespace hc::editor
{
  /**
   * @brief Abstract base class for ImGui-based window views.
   *
   * Provides a common interface and state for ImGui windows, including
   * open/close management.
   */
  class AWindowView : public ABaseView
  {
  public:
    virtual ~AWindowView();

    /**
     * @brief Draws the window contents.
     */
    void draw() override;

    /**
     * @brief Returns the display name of the window.
     *
     * This name is used as the title in the ImGui window and should be
     * unique for each window type.
     *
     * @return The window's display name.
     */
    const String& getWindowName() const;

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

    /**
     * @brief Returns the current total size of the window, including title bar and
     * borders.
     *
     * @return The total window size as a Vector2f (width, height).
     */
    const Vector2f& getWindowSize() const;

    /**
     * @brief Returns the available content area size of the window, excluding title bar,
     * borders and scrollbars.
     *
     * @return The content area size as a Vector2f (width, height).
     */
    const Vector2f& getContentSize() const;
    
    /**
     * @brief Returns the current position of the window.
     *
     * @return The window position as a Vector2f (x, y).
     */
    const Vector2f& getWindowPosition() const;

  protected:
    String m_windowName;
    Vector2f m_windowSize;
    Vector2f m_contentSize;
    Vector2f m_windowPosition;
    bool m_isOpen = true;

    /**
     * @brief Constructs a WindowView with the given name.
     *
     * @param name The window's display name.
     * @param isOpen Initial open state of the window.
     */
    AWindowView(const String& name, bool isOpen = false);

    /**
     * @brief Pure virtual method for drawing the window's specific contents.
     *
     * Derived classes must implement this to define their UI.
     */
    virtual void onDraw() = 0;

  private:

    /**
     * @brief Updates the window's state (size, position, content size) based on ImGui's
     * current state.
     */
    void updateWindowState();
  };
}
