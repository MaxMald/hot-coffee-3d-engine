#pragma once

#include "hc/hcWindowSfmlPrerequisites.h"

namespace hc
{
  class SfmlWindow;

  class SfmlWindowManager : public IWindowManager
  {
  public:
    SfmlWindowManager();
    ~SfmlWindowManager() override;

    bool createWindow(const WindowSettings& settings) override;
    IWindow& getWindow() const override;

  private:
    UniquePtr<SfmlWindow> m_window;
  };
}
