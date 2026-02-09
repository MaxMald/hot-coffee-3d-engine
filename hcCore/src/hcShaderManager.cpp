#include "hc/hcShaderManager.h"
#include "hc/hcIShaderFactory.h"

namespace hc
{
  static constexpr const char* DEFAULT_VERTEX_SHADER_KEY = "#_HC_DEFAULT_VERTEX_SHADER";
  static constexpr const char* UNLIT_FRAGMENT_SHADER_KEY = "#_HC_UNLIT_FRAGMENT_SHADER";

  ShaderManager::ShaderManager(IShaderFactory& shaderFactory) :
    m_shaderFactory(shaderFactory)
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
      return nullptr;

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

    SharedPtr<IShader> shader = m_shaderFactory.createShaderFromStringContent(
      shaderCode,
      type
    );

    if (!shader)
      return nullptr;

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
    SharedPtr<IShader> shader = m_shaderFactory.createDefaultVertexShader();
    cacheResource(DEFAULT_VERTEX_SHADER_KEY, shader);
  }

  void ShaderManager::createUnlitFragmentShader()
  {
    SharedPtr<IShader> shader = m_shaderFactory.createUnlitFragmentShader();
    cacheResource(UNLIT_FRAGMENT_SHADER_KEY, shader);
  }
}
