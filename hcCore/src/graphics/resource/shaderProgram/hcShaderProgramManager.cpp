#include "hc/graphics/resource/shaderProgram/hcShaderProgramManager.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgramFactory.h"
#include "hc/graphics/resource/shader/hcIShaderManager.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgram.h"
#include "hc/graphics/resource/shader/hcIShader.h"

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

  SharedPtr<IShaderProgram> ShaderProgramManager::getBlinnPhongForwardProgram()
  {
    if (!m_blinnPhongForwardProgram)
      createBlinnPhongForwardProgram();
    return m_blinnPhongForwardProgram;
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

  void ShaderProgramManager::createBlinnPhongForwardProgram()
  {
    m_blinnPhongForwardProgram = m_shaderProgramFactory->createShaderProgram();
    if (!m_blinnPhongForwardProgram)
    {
      throw RuntimeErrorException(
        "Failed to create Blinn-Phong forward shader program."
      );
    }

    SharedPtr<IShader> vertexShader = m_shaderManager.getLitVertexShader();
    SharedPtr<IShader> fragmentShader = m_shaderManager.getBlinnPhongForwardFragmentShader();
    if (!vertexShader || !fragmentShader)
    {
      throw RuntimeErrorException(
        "Failed to retrieve shaders for Blinn-Phong forward shader program."
      );
    }

    if (!vertexShader->isCompiled())
      vertexShader->compile();

    if (!fragmentShader->isCompiled())
      fragmentShader->compile();

    m_blinnPhongForwardProgram->attachShader(vertexShader);
    m_blinnPhongForwardProgram->attachShader(fragmentShader);
    m_blinnPhongForwardProgram->linkShaders();
  }
}
