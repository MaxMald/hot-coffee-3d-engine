#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/shaderProgram/hcBuiltInShaderProgramType.h"

namespace hc
{
  class IShaderProgram;
  class IShader;

  /**
   * @brief Interface for shader program manager classes.
   *
   * Provides functionality for creating, caching, and retrieving shader
   * programs, including custom and built-in shader programs.
   */
  class HC_CORE_EXPORT IShaderProgramManager : public NonCopyable
  {
  public:
    virtual ~IShaderProgramManager() = default;

    /**
     * @brief Creates a custom shader program from the given vertex and fragment shaders.
     *
     * @param programKey Unique key identifying the shader program.
     * @param vertexShader Shared pointer to the vertex shader to attach.
     * @param fragmentShader Shared pointer to the fragment shader to attach.
     *
     * @throws RuntimeErrorException if the shader program creation fails or it already
     * exists with the same key.
     */
    virtual SharedPtr<IShaderProgram> createShaderProgram(
      const String& programKey,
      SharedPtr<IShader> vertexShader,
      SharedPtr<IShader> fragmentShader
    ) = 0;

    /**
     * @brief Retrieves a custom shader program by its key.
     *
     * @param programKey Unique key identifying the shader program.
     *
     * @return Shared pointer to the custom shader program, or nullptr if not found.
     */
    virtual SharedPtr<IShaderProgram> getShaderProgram(
      const String& programKey
    ) const = 0;

    /**
     * @brief Checks if a custom shader program with the specified key exists.
     *
     * @param programKey Unique key identifying the shader program.
     *
     * @return True if the custom shader program exists, false otherwise.
     */
    virtual bool hasShaderProgram(const String& programKey) const = 0;

    /**
     * @brief Retrieves a built-in shader program by its type. Creates and caches the
     * built-in shader program on first request.
     *
     * @param type The type of the built-in shader program to retrieve.
     *
     * @return Shared pointer to the built-in shader program.
     *
     * @throws RuntimeErrorException if the shader program for the specified type is not
     * implemented.
     */
    virtual SharedPtr<IShaderProgram> getBuiltInShaderProgram(
      builtInShaderProgramType::Type type
    ) = 0;

    /**
     * @brief Clears all managed shader programs and releases associated
     * resources.
     */
    virtual void clear() = 0;
  };
}
