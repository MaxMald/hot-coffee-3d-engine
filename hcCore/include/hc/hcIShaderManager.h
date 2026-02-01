#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcShaderStageType.h"

namespace hc
{
  class IShader;

  class IShaderManager : public NonCopyable
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
     * @brief Clears all cached shaders from the manager.
     */
    virtual void clear() = 0;
  };
}
