#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IShaderProgram;
  class IGraphicsManager;
  class HotCoffeeEngine;

  class ShaderProgramManager :
    public NonCopyable,
    public IDependencyResolvable
  {
  public:
    ShaderProgramManager() = default;
    ~ShaderProgramManager() = default;

    void resolveDependencies(DependencyContainer& container) override;
    SharedPtr<IShaderProgram> getUnlit();
    void destroy();

  private:
    SharedPtr<IGraphicsManager> m_graphicsManager;
    SharedPtr<IShaderProgram> m_unlitShaderProgram;

    void createPredefinedShaderPrograms();

    friend class HotCoffeeEngine;
  };
}
