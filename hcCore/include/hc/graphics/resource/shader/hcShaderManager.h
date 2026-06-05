#pragma once

#include "hc/graphics/resource/shader/hcIShaderManager.h"
#include "hc/graphics/resource/hcResourcesCache.h"

namespace hc
{
  class IShaderFactory;

  /**
   * @brief Manages shader resources and their creation, caching, and retrieval.
   *
   * Provides functionality for creating shaders from files or string content,
   * caching shader instances, and accessing built-in shaders.
   */
  class HC_CORE_EXPORT ShaderManager : public IShaderManager
  {
  public:
    /**
     * @brief Constructs a ShaderManager with the required shader factory.
     *
     * @param shaderFactory Unique pointer to the shader factory used for shader
     * creation.
     */
    ShaderManager(UniquePtr<IShaderFactory> shaderFactory);
    ~ShaderManager() override;

    /**
     * @copydoc IShaderManager::createShaderFromFile
     */
    SharedPtr<IShader> createShaderFromFile(
      const Path& shaderPath,
      shaderStageType::Type type
    ) override;

    /**
     * @copydoc IShaderManager::createShaderFromString
     */
    SharedPtr<IShader> createShaderFromString(
      const String& shaderKey,
      const String& shaderCode,
      shaderStageType::Type type
    ) override;

    /**
     * @copydoc IShaderManager::getShader
     */
    SharedPtr<IShader> getShader(const String& shaderKey) const override;

    /**
     * @copydoc IShaderManager::hasShader
     */
    bool hasShader(const String& shaderKey) const override;

    /**
     * @copydoc IShaderManager::getBuiltInShader
     */
    SharedPtr<IShader> getBuiltInShader(const builtInShaderType::Type type) override;

    /**
     * @brief Clears all cached shaders from the manager.
     */
    void clear() override;

  private:
    UniquePtr<IShaderFactory> m_shaderFactory;
    UnorderedMap<builtInShaderType::Type, SharedPtr<IShader>> m_builtInShaders;
    UnorderedMap<String, SharedPtr<IShader>> m_customShaders;
  };
}
