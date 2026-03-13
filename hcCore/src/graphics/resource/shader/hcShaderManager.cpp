#include "hc/graphics/resource/shader/hcShaderManager.h"
#include "hc/graphics/resource/shader/hcIShaderFactory.h"

namespace hc
{
  static constexpr const char* DEFAULT_VERTEX_SHADER_KEY = "#_HC_DEFAULT_VERTEX_SHADER";
  static constexpr const char* UNLIT_FRAGMENT_SHADER_KEY = "#_HC_UNLIT_FRAGMENT_SHADER";

  ShaderManager::ShaderManager(UniquePtr<IShaderFactory> shaderFactory) :
    m_shaderFactory(std::move(shaderFactory))
  {
  }

  ShaderManager::~ShaderManager()
  {
  }

  SharedPtr<IShader> ShaderManager::createShaderFromFile(
    const Path& shaderPath,
    shaderStageType::Type type
  )
  {
    String shaderKey = shaderPath.string();
    if (hasCachedResource(shaderKey))
      return getCachedResource(shaderKey);

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
    if (hasCachedResource(shaderKey))
      return getCachedResource(shaderKey);

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

    cacheResource(shaderKey, shader);
    return shader;
  }

  SharedPtr<IShader> ShaderManager::getShader(
    const String& shaderKey
  ) const
  {
    return getCachedResource(shaderKey);
  }

  SharedPtr<IShader> ShaderManager::getDefaultVertexShader()
  {
    if (!hasCachedResource(DEFAULT_VERTEX_SHADER_KEY))
      createDefaultVertexShader();
    return getCachedResource(DEFAULT_VERTEX_SHADER_KEY);
  }

  SharedPtr<IShader> ShaderManager::getUnlitFragmentShader()
  {
    if (!hasCachedResource(UNLIT_FRAGMENT_SHADER_KEY))
      createUnlitFragmentShader();
    return getCachedResource(UNLIT_FRAGMENT_SHADER_KEY);
  }

  void ShaderManager::clear()
  {
    clearCache();
  }

  void ShaderManager::createDefaultVertexShader()
  {
    SharedPtr<IShader> shader = m_shaderFactory->createDefaultVertexShader();
    if (!shader)
    {
      throw RuntimeErrorException(
        "Failed to create default vertex shader."
      );
    }

    cacheResource(DEFAULT_VERTEX_SHADER_KEY, shader);
  }

  void ShaderManager::createUnlitFragmentShader()
  {
    SharedPtr<IShader> shader = m_shaderFactory->createUnlitFragmentShader();
    if (!shader)
    {
      throw RuntimeErrorException(
        "Failed to create unlit fragment shader."
      );
    }

    cacheResource(UNLIT_FRAGMENT_SHADER_KEY, shader);
  }
}
