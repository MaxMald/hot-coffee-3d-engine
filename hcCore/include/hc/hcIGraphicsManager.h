#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIWindow.h"

namespace hc
{
  class IDrawable;
  class HotCoffeeEngine;

  class HC_CORE_EXPORT IGraphicsManager : public NonCopyable
  {
  public:
    virtual ~IGraphicsManager();

    virtual void beginFrame() = 0;
    virtual void draw(IDrawable&) = 0;
    virtual void endFrame(IWindow&) = 0;

  protected:
    IGraphicsManager();

    virtual void init() = 0;
    virtual void destroy() = 0;

    friend HotCoffeeEngine;
  };
}
