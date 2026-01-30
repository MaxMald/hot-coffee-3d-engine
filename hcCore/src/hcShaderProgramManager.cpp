#include "hc/hcShaderProgramManager.h"
#include "hc/hcDependencyContainer.h"
#include "hc/hcIGraphicsManager.h"
#include "hc/hcIShaderProgram.h"

namespace hc
{
  void ShaderProgramManager::resolveDependencies(
    DependencyContainer& container
  )
  {
    m_graphicsManager = container.resolve<IGraphicsManager>();
  }

  SharedPtr<IShaderProgram> ShaderProgramManager::getUnlit()
  {
    if (!m_unlitShaderProgram)
      return nullptr;
    return m_unlitShaderProgram;
  }

  void ShaderProgramManager::destroy()
  {
    if (m_unlitShaderProgram)
      m_unlitShaderProgram->destroy();
  }

  void ShaderProgramManager::createPredefinedShaderPrograms()
  {
    m_unlitShaderProgram = m_graphicsManager->createUnlitShaderProgram();
  }
}
