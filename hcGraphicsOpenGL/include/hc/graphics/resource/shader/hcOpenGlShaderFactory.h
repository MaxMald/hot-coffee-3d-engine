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
     * @copydoc IShaderFactory::createDefaultVertexShader
     */
    SharedPtr<IShader> createDefaultVertexShader() override;

    /**
     * @copydoc IShaderFactory::createLitVertexShader
     */
    SharedPtr<IShader> createLitVertexShader() override;

    /**
     * @copydoc IShaderFactory::createUnlitFragmentShader
     */
    SharedPtr<IShader> createUnlitFragmentShader() override;

    /**
     * @copydoc IShaderFactory::createBlinnPhongForwardFragmentShader
     */
    SharedPtr<IShader> createBlinnPhongForwardFragmentShader() override;

    /**
     * @copydoc IShaderFactory::createBlinnPhongDeferredFragmentShader
     */
    SharedPtr<IShader> createBlinnPhongDeferredFragmentShader() override;
  };
}
