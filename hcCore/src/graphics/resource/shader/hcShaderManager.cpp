#include "hc/graphics/resource/shader/hcShaderManager.h"
#include "hc/graphics/resource/shader/hcIShaderFactory.h"

namespace hc
{
  ShaderManager::ShaderManager(UniquePtr<IShaderFactory> shaderFactory) :
    m_shaderFactory(std::move(shaderFactory)),
    m_builtInShaders(),
    m_customShaders()
  {}

  ShaderManager::~ShaderManager()
  {
    clear();
  }

  SharedPtr<IShader> ShaderManager::createShaderFromFile(
    const Path& shaderPath,
    shaderStageType::Type type
  )
  {
    String shaderKey = shaderPath.string();
    if (m_customShaders.find(shaderKey) != m_customShaders.end())
      return m_customShaders[shaderKey];

    Optional<String> shaderCode = fileUtilities::LoadStringFromFile(
      shaderPath
    );

    if (!shaderCode.has_value())
    {
      LogService::Error(
        String::Format(
          "Failed to load shader from file: %s",
          shaderPath.string().c_str()
        )
      );
      return nullptr;
    }

    return createShaderFromString(
      shaderKey,
      shaderCode.value(),
      type
    );
  }

  SharedPtr<IShader> ShaderManager::createShaderFromString(
    const String& shaderKey,
    const String& shaderCode,
    shaderStageType::Type type
  )
  {
    if (hasShader(shaderKey))
    {
      throw RuntimeErrorException(
        String::Format(
          "Shader with key '%s' already exists in the cache.",
          shaderKey.c_str()
        )
      );
    }

    SharedPtr<IShader> shader = m_shaderFactory->createShaderFromStringContent(
      shaderCode,
      type
    );

    if (!shader)
    {
      LogService::Error(
        String::Format(
          "Failed to create shader from string with key: %s",
          shaderKey.c_str()
        )
      );
      return nullptr;
    }

    m_customShaders.insert({ shaderKey, shader });
    return shader;
  }

  SharedPtr<IShader> ShaderManager::getShader(const String& shaderKey) const
  {
    if (m_customShaders.find(shaderKey) != m_customShaders.end())
      return m_customShaders.at(shaderKey);
    return nullptr;
  }

  bool ShaderManager::hasShader(const String& shaderKey) const
  {
    return m_customShaders.find(shaderKey) != m_customShaders.end();
  }

  SharedPtr<IShader> ShaderManager::getBuiltInShader(const builtInShaderType::Type type)
  {
    if (m_builtInShaders.find(type) != m_builtInShaders.end())
      return m_builtInShaders[type];

    SharedPtr<IShader> shader = m_shaderFactory->createBuiltInShaderType(type);
    if (!shader)
    {
      throw RuntimeErrorException(
        String::Format(
          "Failed to create built-in shader of type: %d",
          static_cast<int>(type)
        )
      );
    }

    m_builtInShaders.insert({ type, shader });
    return shader;
  }

  void ShaderManager::clear()
  {
    m_builtInShaders.clear();
    m_customShaders.clear();
  }
}
