#include "hc/hcOpenGlShaderManager.h"
#include "hc/hcOpenGlShader.h"

namespace hc
{
  OpenGlShaderManager::OpenGlShaderManager()
  {
  }

  OpenGlShaderManager::~OpenGlShaderManager()
  {
  }

  SharedPtr<IShader> OpenGlShaderManager::createShaderFromFile(
    const Path& shaderPath,
    shaderStageType::Type type
  )
  {
    String shaderKey = shaderPath.string();
    auto it = m_cachedShaders.find(shaderKey);
    if (it != m_cachedShaders.end())
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

  SharedPtr<IShader> OpenGlShaderManager::createShaderFromString(
    const String& shaderKey,
    const String& shaderCode,
    shaderStageType::Type type
  )
  {
    auto it = m_cachedShaders.find(shaderKey);
    if (it != m_cachedShaders.end())
    {
      return it->second;
    }

    SharedPtr<OpenGlShader> shader = MakeShared<OpenGlShader>(type, shaderCode);
    m_cachedShaders[shaderKey] = shader;
    return shader;
  }

  SharedPtr<IShader> OpenGlShaderManager::getShader(
    const String& shaderKey
  ) const
  {
    auto it = m_cachedShaders.find(shaderKey);
    if (it != m_cachedShaders.end())
    {
      return it->second;
    }
    return nullptr;
  }

  void OpenGlShaderManager::clear()
  {
    m_cachedShaders.clear();
  }
}
