#pragma once

#include "hc/hcIWindow.h"

namespace hc
{
  class IWindowFactory
  {
  public:
    virtual ~IWindowFactory() = default;

    virtual IWindow* createWindow() = 0;
  };
}
