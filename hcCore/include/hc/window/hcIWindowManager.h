#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/window/hcWindowSettings.h"

namespace hc
{
  class IWindow;

  /**
   * @brief Interface for window manager objects.
   *
   * Provides methods for creating and accessing windows. Implementations
   * must handle window creation and provide access to the managed window.
   */
  class HC_CORE_EXPORT IWindowManager : public NonCopyable
  {
  public:
    virtual ~IWindowManager() = default;

    /**
     * @brief Creates a window with the specified settings.
     *
     * @param settings The settings used to configure the window.
     * @return True if the window was created successfully, false otherwise.
     */
    virtual bool createWindow(const WindowSettings& settings) = 0;

    /**
     * @brief Returns a reference to the managed window.
     *
     * @return Reference to the window instance.
     */
    virtual IWindow& getWindow() const = 0;

    /**
     * @brief Destroys the managed window and releases associated resources.
     */
    virtual void destroy() = 0;

  protected:
    IWindowManager() = default;
  };
}
