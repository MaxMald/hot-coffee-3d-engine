#include "hc/graphics/resource/shader/hcAShaderManager.h"

namespace hc
{
  AShaderManager::AShaderManager() :
    m_builtInShaders(),
    m_customShaders()
  {}

  AShaderManager::~AShaderManager()
  {}

  SharedPtr<IShader> AShaderManager::createShaderFromFile(
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

  SharedPtr<IShader> AShaderManager::createShaderFromString(
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

    try
    {
      SharedPtr<IShader> shader = createConcreteShaderFromString(
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
    catch (const RuntimeErrorException& e)
    {
      LogService::Error(
        String::Format(
          "Failed to create shader from string with key: %s. Error: %s",
          shaderKey.c_str(),
          e.what()
        )
      );

      return nullptr;
    }
  }

  SharedPtr<IShader> AShaderManager::getShader(const String& shaderKey) const
  {
    if (m_customShaders.find(shaderKey) != m_customShaders.end())
      return m_customShaders.at(shaderKey);
    return nullptr;
  }

  bool AShaderManager::hasShader(const String& shaderKey) const
  {
    return m_customShaders.find(shaderKey) != m_customShaders.end();
  }

  SharedPtr<IShader> AShaderManager::getBuiltInShader(const builtInShaderType::Type type)
  {
    if (m_builtInShaders.find(type) != m_builtInShaders.end())
      return m_builtInShaders[type];

    try
    {
      SharedPtr<IShader> shader = createConcreteBuiltInShader(type);

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
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "Failed to create built-in shader of type: %d. Error: %s",
          static_cast<int>(type),
          e.what()
        )
      );

      return nullptr;
    }
  }

  void AShaderManager::clear()
  {
    m_builtInShaders.clear();
    m_customShaders.clear();
    onClear();
  }

  void AShaderManager::destroy()
  {
    clear();
    onDestroy();
  }
}
