#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIWindow.h"

namespace hc
{
  class HC_CORE_EXPORT IGraphicsManager : public NonCopyable
  {
  public:
    virtual ~IGraphicsManager();

    virtual void init() = 0;
    virtual void draw(IWindow&) = 0;
    virtual void destroy() = 0;

  protected:
    IGraphicsManager();
  };
}
