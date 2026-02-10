#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcShaderStageType.h"

namespace hc
{
  class IShader;

  /**
   * @brief Interface for shader manager classes.
   *
   * Provides methods for creating, retrieving, and caching shader instances.
   * Supports creation from files and string content, access to default shaders,
   * and management of cached shader resources.
   */
  class HC_CORE_EXPORT IShaderManager : public NonCopyable
  {
  public:
    virtual ~IShaderManager() = default;

    /**
     * @brief Creates a shader from a given file path. If a shader for the file
     * already exists, returns the cached instance.
     * 
     * @param shaderPath Path to the shader file.
     * @param type The type of the shader stage.
     *
     * @return Shared pointer to the created or cached shader, or nullptr on
     * failure.
     */
    virtual SharedPtr<IShader> createShaderFromFile(
      const Path& shaderPath,
      shaderStageType::Type type
    ) = 0;

    /**
     * @brief Creates a shader from given shader code. If a shader for the code
     * already exists, returns the cached instance.
     *
     * @param shaderKey Unique key identifying the shader.
     * @param shaderCode The source code of the shader.
     * @param type The type of the shader stage.
     *
     * @return Shared pointer to the created or cached shader, or nullptr on
     * failure.
     */
    virtual SharedPtr<IShader> createShaderFromString(
      const String& shaderKey,
      const String& shaderCode,
      shaderStageType::Type type
    ) = 0;

    /**
     * @brief Retrieves a cached shader by its key.
     *
     * @param shaderKey Unique key identifying the shader.
     *
     * @return Shared pointer to the cached shader, or nullptr if not found.
     */
    virtual SharedPtr<IShader> getShader(
      const String& shaderKey
    ) const = 0;

    /**
     * @brief Retrieves the default vertex shader. If it does not exist, creates
     * and caches it before returning.
     */
    virtual SharedPtr<IShader> getDefaultVertexShader() = 0;
    
    /**
     * @brief Retrieves the unlit fragment shader. If it does not exist, creates
     * and caches it before returning.
     */
    virtual SharedPtr<IShader> getUnlitFragmentShader() = 0;

    /**
     * @brief Clears all cached shaders from the manager.
     */
    virtual void clear() = 0;
  };
}
