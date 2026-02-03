#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIDrawable.h"

namespace hc
{
  class Model;

  class IMesh : public NonCopyable, public IDrawable
  {
  public:
    virtual ~IMesh() = default;

    virtual SharedPtr<Model> getModel() const = 0;
    virtual void update() = 0;
    virtual void destroy() = 0;

  protected:
    IMesh() = default;
  };
}
