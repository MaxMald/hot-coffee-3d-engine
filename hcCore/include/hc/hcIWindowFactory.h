#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIWindow.h"

namespace hc
{
  /**
   * @brief Interface for window factory classes.
   *
   * Provides an abstract interface for creating window instances.
   * Implementations should return platform- or backend-specific IWindow objects.
   */
  class HC_CORE_EXPORT IWindowFactory
  {
  public:
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~IWindowFactory() = default;

    /**
     * @brief Creates a new window instance with the specified settings.
     * 
     * @param settings The settings to use for window creation.
     * 
     * @returns A pointer to the created IWindow instance.
     */
    virtual SharedPtr<IWindow> createWindow(const WindowSettings& settings) = 0;
  };
}
