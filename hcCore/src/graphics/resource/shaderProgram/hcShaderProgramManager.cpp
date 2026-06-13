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
      throw InvalidArgumentException(
        String::Format("Invalid shaders provided for shader program with key '%s'.", programKey.c_str())
      );
    }

    Vector<SharedPtr<IShader>> shaders = { vertexShader, fragmentShader };
    shaderProgram->initialize(shaders);

    if (!shaderProgram->isValid())
      throw RuntimeErrorException(
        String::Format("Failed to initialize shader program with key '%s'.", programKey.c_str())
      );

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
    else if (type == builtInShaderProgramType::Skybox)
    {
      createBuiltInShaderProgram(
        type,
        m_shaderManager.getBuiltInShader(builtInShaderType::SkyboxVertex),
        m_shaderManager.getBuiltInShader(builtInShaderType::SkyboxFragment)
      );
    }
    else if (type == builtInShaderProgramType::FinalPass)
    {
      createBuiltInShaderProgram(
        type,
        m_shaderManager.getBuiltInShader(builtInShaderType::FullScreenTriangleVertex),
        m_shaderManager.getBuiltInShader(builtInShaderType::FinalPassFragment)
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
      throw InvalidArgumentException(
        String::Format("Failed to retrieve shaders for built-in shader program type %d.", type)
      );
    }

    Vector<SharedPtr<IShader>> shaders = { vertexShader, fragmentShader };
    shaderProgram->initialize(shaders);
    if (!shaderProgram->isValid())
      throw RuntimeErrorException(
        String::Format("Failed to initialize shader program for built-in type %d.", type)
      );

    m_builtInShaderPrograms[type] = shaderProgram;
  }
}
