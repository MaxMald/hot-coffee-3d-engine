#pragma once

#include "hc/hcWindowSfmlPrerequisites.h"
#include "hc/hcIWindowFactory.h"

namespace hc
{
  class HC_WINDOW_SFML_EXPORT SfmlWindowFactory : public IWindowFactory
  {
  public:
    SfmlWindowFactory();
    virtual ~SfmlWindowFactory();

    SharedPtr<IWindow> createWindow(const WindowSettings& settings) override;
  };
}
