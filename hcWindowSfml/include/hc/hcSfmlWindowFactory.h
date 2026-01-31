#pragma once

#include "hc/hcWindowSfmlPrerequisites.h"

namespace hc
{
  /**
   * @brief Factory class for creating SFML-based window instances.
   */
  class HC_WINDOW_SFML_EXPORT SfmlWindowFactory : public IWindowFactory
  {
  public:
    /**
     * @brief Default constructor.
     */
    SfmlWindowFactory();

    /**
     * @brief Destructor.
     */
    virtual ~SfmlWindowFactory();

    /**
     * @copydoc IWindowFactory::createWindow
     */
    SharedPtr<IWindow> createWindow(const WindowSettings& settings) override;
  };
}
