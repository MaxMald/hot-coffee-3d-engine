#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/hcGraphicsCommons.h"

namespace hc
{
  class IShader;

  /**
   * @brief Interface for shader manager classes.
   *
   * Provides functionality for creating shaders from files or string content,
   * caching shader instances, and accessing built-in shaders.
   */
  class HC_CORE_EXPORT IShaderManager : public NonCopyable
  {
  public:
    virtual ~IShaderManager() = default;

    /**
     * @brief Initializes the shader manager.
     *
     * This method should be called before using the shader manager to ensure
     * that any necessary resources or configurations are set up.
     */
    virtual void initialize() = 0;

    /**
     * @brief Creates a custom shader from a given file path.
     * 
     * @param shaderPath Path to the shader file.
     * @param type The type of the shader stage.
     *
     * @return Shared pointer to the created shader.
     *
     * @throws RuntimeErrorException if the shader file cannot be loaded or the shader
     * creation fails.
     */
    virtual SharedPtr<IShader> createShaderFromFile(
      const Path& shaderPath,
      shaderStageType::Type type
    ) = 0;

    /**
     * @brief Creates a custom shader from given shader code.
     *
     * @param shaderKey Unique key identifying the shader.
     * @param shaderCode The source code of the shader.
     * @param type The type of the shader stage.
     *
     * @return Shared pointer to the created shader.
     *
     * @throws RuntimeErrorException if the shader creation fails.
     */
    virtual SharedPtr<IShader> createShaderFromString(
      const String& shaderKey,
      const String& shaderCode,
      shaderStageType::Type type
    ) = 0;

    /**
     * @brief Retrieves a custom shader by its key.
     *
     * @param shaderKey Unique key identifying the shader.
     *
     * @return Shared pointer to the custom shader, or nullptr if not found.
     */
    virtual SharedPtr<IShader> getShader(const String& shaderKey) const = 0;

    /**
     * @brief Checks if a custom shader with the given key exists in the cache.
     *
     * @param shaderKey Unique key identifying the shader.
     *
     * @return True if the custom shader exists in the cache, false otherwise.
     */
    virtual bool hasShader(const String& shaderKey) const = 0;

    /**
     * @brief Retrieves a built-in shader by its type. If the shader does not
     * exist, creates and caches it before returning.
     *
     * @param type The type of the built-in shader to retrieve.
     *
     * @return Shared pointer to the built-in shader instance.
     *
     * @throws RuntimeErrorException if the built-in shader for the specified type does
     * not exist or is not implemented.
     */
    virtual SharedPtr<IShader> getBuiltInShader(builtInShaderType::Type type) = 0;

    /**
     * @brief Clears all cached shaders from the manager.
     */
    virtual void clear() = 0;

    /**
     * @brief Destroys the shader manager and releases any associated resources.
     *
     * This method should be called when the shader manager is no longer needed
     * to ensure proper cleanup of resources.
     */
    virtual void destroy() = 0;
  };
}
