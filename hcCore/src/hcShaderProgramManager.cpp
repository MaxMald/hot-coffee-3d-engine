#include "hc/hcShaderProgramManager.h"
#include "hc/hcIShaderProgramFactory.h"
#include "hc/hcIShaderManager.h"
#include "hc/hcIShaderProgram.h"
#include "hc/hcIShader.h"

namespace hc
{
  ShaderProgramManager::ShaderProgramManager(
    UniquePtr<IShaderProgramFactory> shaderProgramFactory,
    IShaderManager& shaderManager
  ) :
    m_shaderProgramFactory(std::move(shaderProgramFactory)),
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
    m_unlitShaderProgram = m_shaderProgramFactory->createShaderProgram();
    if (!m_unlitShaderProgram)
    {
      throw RuntimeErrorException(
        "Failed to create unlit shader program."
      );
    }

    SharedPtr<IShader> vertexShader = m_shaderManager.getDefaultVertexShader();
    SharedPtr<IShader> fragmentShader = m_shaderManager.getUnlitFragmentShader();

    if (!vertexShader || !fragmentShader)
    {
      throw RuntimeErrorException(
        "Failed to retrieve shaders for unlit shader program."
      );
    }

    if (!vertexShader->isCompiled())
      vertexShader->compile();
    if (!fragmentShader->isCompiled())
      fragmentShader->compile();

    m_unlitShaderProgram->attachShader(vertexShader);
    m_unlitShaderProgram->attachShader(fragmentShader);

    m_unlitShaderProgram->linkShaders();
  }
}
