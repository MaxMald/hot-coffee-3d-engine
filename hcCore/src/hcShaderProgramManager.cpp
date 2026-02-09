#include "hc/hcShaderProgramManager.h"
#include "hc/hcIShaderProgramFactory.h"
#include "hc/hcIShaderManager.h"

namespace hc
{
  ShaderProgramManager::ShaderProgramManager(
    IShaderProgramFactory& shaderProgramFactory,
    IShaderManager& shaderManager
  ) : m_shaderProgramFactory(shaderProgramFactory),
    m_shaderManager(shaderManager)
  {
  }

  ShaderProgramManager::~ShaderProgramManager()
  {
  }

  SharedPtr<IShaderProgram> ShaderProgramManager::getUnlitShaderProgram()
  {
    if (!m_unlitShaderProgram)
      createUnlitShaderProgram();
    return m_unlitShaderProgram;
  }

  void ShaderProgramManager::clear()
  {
    m_unlitShaderProgram.reset();
  }

  void ShaderProgramManager::createUnlitShaderProgram()
  {
    Vector<SharedPtr<IShader>> shaders;
    shaders.push_back(m_shaderManager.getDefaultVertexShader());
    shaders.push_back(m_shaderManager.getUnlitFragmentShader());
    m_unlitShaderProgram = m_shaderProgramFactory.createShaderProgram(shaders);
  }
}
