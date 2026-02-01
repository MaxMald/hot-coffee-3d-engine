#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlShader;

  class OpenGlShaderManager : public IShaderManager
  {
  public:
    OpenGlShaderManager();
    ~OpenGlShaderManager() override;

    SharedPtr<IShader> createShaderFromFile(
      const Path& shaderPath,
      shaderStageType::Type type
    ) override;

    SharedPtr<IShader> createShaderFromString(
      const String& shaderKey,
      const String& shaderCode,
      shaderStageType::Type type
    ) override;

    SharedPtr<IShader> getShader(
      const String& shaderKey
    ) const override;

    void clear() override;

  private:
    UnorderedMap<String, SharedPtr<OpenGlShader>> m_cachedShaders;
  };
}
