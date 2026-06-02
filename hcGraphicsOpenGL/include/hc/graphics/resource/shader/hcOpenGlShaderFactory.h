#pragma once

#include <hc/graphics/resource/shader/hcIShaderFactory.h>

namespace hc
{
  /**
   * @brief Factory for creating OpenGL shader instances.
   *
   * Implements the IShaderFactory interface to construct OpenGL shader objects
   * from source code strings or by providing default shader implementations.
   */
  class OpenGlShaderFactory : public IShaderFactory
  {
  public:
    OpenGlShaderFactory() = default;
    ~OpenGlShaderFactory() override = default;

    /**
     * @copydoc IShaderFactory::createShaderFromStringContent
     */
    SharedPtr<IShader> createShaderFromStringContent(
      const String& content,
      shaderStageType::Type type
    ) override;

    /**
     * @copydoc IShaderFactory::createBuiltInShaderType
     */
    SharedPtr<IShader> createBuiltInShaderType(
      builtInShaderType::Type type
    ) override;
  };
}
