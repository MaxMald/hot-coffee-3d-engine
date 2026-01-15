#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcVector2.h"
#include "hc/hcWindowSettings.h"

namespace hc
{
  /**
   * @brief Interface for window management.
   *
   * Provides an abstract interface for creating, destroying, and manipulating a
   * window's size and position. Implementations should provide platform-specific
   * or backend-specific window handling.
   */
  class IWindow
  {
  public:
    virtual ~IWindow() = default;

    /**
     * @brief Creates the window with the specified settings.
     * 
     * @param settings The settings to use for window creation.
     */
    virtual void create(const WindowSettings& settings) = 0;

    /**
     * @brief Destroys the window and releases any associated resources.
     */
    virtual void destroy() = 0;

    /**
     * @brief Gets the current size of the window.
     * 
     * @return The size of the window as a Vector2u (width, height).
     */
    virtual Vector2u getSize() const = 0;

    /**
     * @brief Sets the size of the window.
     * 
     * @param size The new size of the window as a Vector2u (width, height).
     */
    virtual void setSize(const Vector2u& size) = 0;

    /**
     * @brief Gets the current position of the window on the screen.
     *
     * @return The position of the window as a Vector2i (x, y).
     */
    virtual Vector2i getPosition() const = 0;

    /**
     * @brief Sets the position of the window on the screen.
     * 
     * @param position The new position of the window as a Vector2i (x, y).
     */
    virtual void setPosition(const Vector2i& position) = 0;

    /**
     * @brief Checks if the window is currently open.
     * 
     * @return True if the window is open, false otherwise.
     */
    virtual bool isOpen() const = 0;
  };
}
