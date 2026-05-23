#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/shader/hcShaderStageType.h"

namespace hc
{
  class IShader;

  /**
   * @brief Interface for shader factory classes.
   *
   * Provides methods for creating shader instances from string content or
   * default types. Implementations should supply concrete shader creation logic.
   */
  class IShaderFactory
  {
  public:
    virtual ~IShaderFactory() = default;

    /**
     * @brief Creates a shader from the given string content and stage type.
     *
     * @param content The shader source code as a string.
     * @param type The shader stage type (e.g., vertex, fragment).
     *
     * @return Shared pointer to the created shader instance. nullptr if creation
     * fails.
     */
    virtual SharedPtr<IShader> createShaderFromStringContent(
      const String& content,
      shaderStageType::Type type
    ) = 0;

    /**
     * @brief Creates a default vertex shader.
     * 
     * @return Shared pointer to the created vertex shader instance.
     */
    virtual SharedPtr<IShader> createDefaultVertexShader() = 0;

    /**
     * @brief Creates a lit vertex shader.
     *
     * @return Shared pointer to the created vertex shader instance.
     */
    virtual SharedPtr<IShader> createLitVertexShader() = 0;

    /**
     * @brief Creates an unlit fragment shader.
     * 
     * @return Shared pointer to the created fragment shader instance.
     */
    virtual SharedPtr<IShader> createUnlitFragmentShader() = 0;

    /**
     * @brief Creates a Blinn-Phong forward fragment shader.
     *
     * @return Shared pointer to the created fragment shader instance.
     */
    virtual SharedPtr<IShader> createBlinnPhongForwardFragmentShader() = 0;

    /**
     * @brief Creates a Blinn-Phong deferred fragment shader.
     *
     * @return Shared pointer to the created fragment shader instance.
     */
    virtual SharedPtr<IShader> createBlinnPhongDeferredFragmentShader() = 0;

  protected:
    IShaderFactory() = default;
  };
}
