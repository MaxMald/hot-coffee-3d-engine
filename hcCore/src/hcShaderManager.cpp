#include "hc/hcShaderManager.h"
#include "hc/hcIGraphicsManager.h"
#include "hc/hcIShader.h"

namespace hc
{
  void ShaderManager::resolveDependencies(DependencyContainer& container)
  {
    m_graphicsManager = container.resolve<IGraphicsManager>();
  }

  SharedPtr<IShader> ShaderManager::createShaderFromFile(
    const Path& shaderPath,
    shaderStageType::Type type
  )
  {
    String shaderKey = shaderPath.string();
    auto it = m_shaderCache.find(shaderKey);
    if (it != m_shaderCache.end())
    {
      return it->second;
    }

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
    auto it = m_shaderCache.find(shaderKey);
    if (it != m_shaderCache.end())
    {
      return it->second;
    }

    if (!m_graphicsManager)
      return nullptr;

    SharedPtr<IShader> shader = m_graphicsManager->createShaderFromString(
      type,
      shaderCode
    );

    if (!shader)
      return nullptr;

    m_shaderCache[shaderKey] = shader;
    return shader;
  }

  SharedPtr<IShader> ShaderManager::getShader(
    const String& shaderKey
  ) const
  {
    auto it = m_shaderCache.find(shaderKey);
    if (it != m_shaderCache.end())
    {
      return it->second;
    }

    return nullptr;
  }
}
