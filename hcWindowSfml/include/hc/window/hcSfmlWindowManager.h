#pragma once

#include "hc/hcWindowSfmlPrerequisites.h"

namespace hc
{
  class SfmlWindow;

  /**
   * @brief Window manager implementation using SFML.
   *
   * Manages the creation, access, and destruction of an SFML-based window.
   */
  class SfmlWindowManager : public IWindowManager
  {
  public:
    SfmlWindowManager();
    ~SfmlWindowManager() override;

    /**
     * @brief Creates an SFML window with the specified settings.
     *
     * @param settings The settings used to configure the window.
     *
     * @return True if the window was created successfully, false otherwise.
     */
    bool createWindow(const WindowSettings& settings) override;

    /**
     * @brief Returns a reference to the managed SFML window.
     *
     * @return Reference to the window instance.
     */
    IWindow& getWindow() const override;

    /**
     * @brief Destroys the managed window and releases associated resources.
     */
    void destroy() override;

  private:
    UniquePtr<SfmlWindow> m_window;
  };
}
