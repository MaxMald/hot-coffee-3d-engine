#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IShaderProgram;

  class IShaderProgramManager : public NonCopyable
  {
  public:
    virtual ~IShaderProgramManager() = default;

    virtual SharedPtr<IShaderProgram> getUnlit() = 0;
    virtual void clear() = 0;
  };
}
