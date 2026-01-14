#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcVector2.h"

namespace hc
{
  class IWindow
  {
  public:
    virtual ~IWindow() = default;

    virtual void create(
      Int32 x,
      Int32 y,
      Int32 width,
      Int32 height,
      const String& title
    ) = 0;
    virtual void destroy() = 0;
    virtual Vector2f getSize() const = 0;
    virtual void setSize(const Vector2f& size) = 0;
    virtual Vector2f getPosition() const = 0;
    virtual void setPosition(const Vector2f& position) = 0;
  };
}
