#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcShaderStageType.h"

namespace hc
{
  class IShader : public NonCopyable
  {
  public:
    virtual ~IShader() = default;

    virtual shaderStageType::Type getStageType() const = 0;
    virtual bool isCompiled() const = 0;
    virtual void compile() = 0;
    virtual void destroy() = 0;
  };
}
