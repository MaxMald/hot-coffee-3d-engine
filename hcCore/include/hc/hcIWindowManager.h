#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcWindowSettings.h"

namespace hc
{
  class IWindow;

  class IWindowManager : public NonCopyable
  {
  public:
    virtual ~IWindowManager() = default;

    virtual bool createWindow(const WindowSettings& settings) = 0;
    virtual IWindow& getWindow() const = 0;

  protected:
    IWindowManager() = default;
  };
}
