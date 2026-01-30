#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"

namespace hc
{
  class IShader : public NonCopyable
  {
  public:
    virtual ~IShader() = default;
  };
}
