#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIDependencyResolvable.h"
#include "hc/hcNonCopyable.h"

namespace hc
{
  class IShader;
  class IGraphicsManager;

  class ShaderManager :
    public NonCopyable,
    public IDependencyResolvable
  {
  public:
    ShaderManager() = default;
    ~ShaderManager() = default;

    /**
     * @copydoc IDependencyResolvable::resolveDependencies
     */
    void resolveDependencies(DependencyContainer& container) override;

    /**
     * @brief Creates a shader from a given file path. If a shader for the file
     * already exists, returns the cached instance.
     *
     * @param key Asset key identifying the shader.
     * @param shaderPath Path to the shader file.
     *
     * @return Shared pointer to the created or cached shader, or nullptr on
     * failure.
     */
    SharedPtr<IShader> createShaderFromFile(
      const Path& shaderPath
    );

    /**
     * @brief Creates a shader from given shader code. If a shader for the code
     * already exists, returns the cached instance.
     *
     * @param shaderKey Unique key identifying the shader.
     * @param shaderCode The source code of the shader.
     *
     * @return Shared pointer to the created or cached shader, or nullptr on
     * failure.
     */
    SharedPtr<IShader> createShaderFromString(
      const String& shaderKey,
      const String& shaderCode
    );

    /**
     * @brief Retrieves a cached shader by its key.
     *
     * @param shaderKey Unique key identifying the shader.
     *
     * @return Shared pointer to the cached shader, or nullptr if not found.
     */
    SharedPtr<IShader> getShader(
      const String& shaderKey
    ) const;

  private:
    SharedPtr<IGraphicsManager> m_graphicsManager;
    UnorderedMap<String, SharedPtr<IShader>> m_shaderCache;
  };
}
