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
    m_shaderManager(shaderManager),
    m_builtInShaderPrograms(),
    m_customShaderPrograms()
  {}

  ShaderProgramManager::~ShaderProgramManager()
  {
    clear();
  }

  SharedPtr<IShaderProgram> ShaderProgramManager::createShaderProgram(
    const String& programKey,
    SharedPtr<IShader> vertexShader,
    SharedPtr<IShader> fragmentShader
  )
  {
    if (hasShaderProgram(programKey))
    {
      throw RuntimeErrorException(
        String::Format("Shader program with key '%s' already exists.", programKey.c_str())
      );
    }

    SharedPtr<IShaderProgram> shaderProgram = m_shaderProgramFactory->createShaderProgram();
    if (!shaderProgram)
    {
      throw RuntimeErrorException(
        String::Format("Failed to create shader program with key '%s'.", programKey.c_str())
      );
    }

    if (!vertexShader || !fragmentShader)
    {
      throw RuntimeErrorException(
        String::Format("Invalid shaders provided for shader program with key '%s'.", programKey.c_str())
      );
    }

    if (!vertexShader->isCompiled())
      vertexShader->compile();
    if (!fragmentShader->isCompiled())
      fragmentShader->compile();

    shaderProgram->attachShader(vertexShader);
    shaderProgram->attachShader(fragmentShader);
    shaderProgram->linkShaders();

    m_customShaderPrograms[programKey] = shaderProgram;
    return shaderProgram;
  }

  SharedPtr<IShaderProgram> ShaderProgramManager::getShaderProgram(
    const String& programKey
  ) const
  {
    if (hasShaderProgram(programKey))
      return m_customShaderPrograms.at(programKey);
    return nullptr;
  }

  bool ShaderProgramManager::hasShaderProgram(const String& programKey) const
  {
    return m_customShaderPrograms.find(programKey) != m_customShaderPrograms.end();
  }

  SharedPtr<IShaderProgram> ShaderProgramManager::getBuiltInShaderProgram(
    builtInShaderProgramType::Type type
  )
  {
    if (m_builtInShaderPrograms.find(type) != m_builtInShaderPrograms.end())
      return m_builtInShaderPrograms.at(type);

    if (type == builtInShaderProgramType::Unlit)
    {
      createBuiltInShaderProgram(
        type,
        m_shaderManager.getBuiltInShader(builtInShaderType::UnlitVertex),
        m_shaderManager.getBuiltInShader(builtInShaderType::UnlitFragment)
      );
    }
    else if (type == builtInShaderProgramType::BlinnPhongForward)
    {
      createBuiltInShaderProgram(
        type,
        m_shaderManager.getBuiltInShader(builtInShaderType::LitVertex),
        m_shaderManager.getBuiltInShader(builtInShaderType::BlinnPhongForwardFragment)
      );
    }
    else if (type == builtInShaderProgramType::BlinnPhongDeferredGeometry)
    {
      createBuiltInShaderProgram(
        type,
        m_shaderManager.getBuiltInShader(builtInShaderType::LitVertex),
        m_shaderManager.getBuiltInShader(builtInShaderType::BlinnPhongDeferredFragment)
      );
    }
    else if (type == builtInShaderProgramType::DeferredLighting)
    {
      createBuiltInShaderProgram(
        type,
        m_shaderManager.getBuiltInShader(builtInShaderType::FullScreenTriangleVertex),
        m_shaderManager.getBuiltInShader(builtInShaderType::DeferredLightingFragment)
      );
    }
    else
    {
      throw RuntimeErrorException(
        String::Format("ShaderProgramManager: Built-in shader program type %d is not implemented.", type)
      );
    }

    return m_builtInShaderPrograms.at(type);
  }

  void ShaderProgramManager::clear()
  {
    m_builtInShaderPrograms.clear();
    m_customShaderPrograms.clear();
  }

  void ShaderProgramManager::createBuiltInShaderProgram(
    builtInShaderProgramType::Type type,
    SharedPtr<IShader> vertexShader,
    SharedPtr<IShader> fragmentShader
  )
  {
    SharedPtr<IShaderProgram> shaderProgram = m_shaderProgramFactory->createShaderProgram();
    if (!shaderProgram)
    {
      throw RuntimeErrorException(
        String::Format("Failed to create shader program for built-in type %d.", type)
      );
    }

    if (!vertexShader || !fragmentShader)
    {
      throw RuntimeErrorException(
        String::Format("Failed to retrieve shaders for built-in shader program type %d.", type)
      );
    }

    if (!vertexShader->isCompiled())
      vertexShader->compile();
    if (!fragmentShader->isCompiled())
      fragmentShader->compile();

    shaderProgram->attachShader(vertexShader);
    shaderProgram->attachShader(fragmentShader);
    shaderProgram->linkShaders();

    m_builtInShaderPrograms[type] = shaderProgram;
  }
}
