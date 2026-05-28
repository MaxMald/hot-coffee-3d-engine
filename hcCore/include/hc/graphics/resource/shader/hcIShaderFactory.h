#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/shader/hcShaderStageType.h"
#include "hc/graphics/resource/shader/hcBuiltInShaderType.h"

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
     * @brief Creates a built-in shader of the specified type.
     *
     * @param type The type of the built-in shader to create.
     *
     * @return Shared pointer to the created built-in shader instance. nullptr if
     * creation fails.
     *
     * @throws RuntimeErrorException if the built-in shader for the specified type does
     * not exist or is not implemented.
     */
    virtual SharedPtr<IShader> createBuiltInShaderType(
      builtInShaderType::Type type
    ) = 0;

  protected:
    IShaderFactory() = default;
  };
}
