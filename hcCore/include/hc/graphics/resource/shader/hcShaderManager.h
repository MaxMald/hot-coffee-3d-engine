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
   * caching shader instances, and accessing default shaders. Implements the
   * IShaderManager interface and uses ResourcesCache for shader caching.
   */
  class HC_CORE_EXPORT ShaderManager :
    public IShaderManager,
    private ResourcesCache<String, IShader>
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
     * @brief Creates a shader from the specified file path. Returns a cached
     * instance if it already exists.
     *
     * @param shaderPath Path to the shader file.
     * @param type The type of the shader stage.
     *
     * @return Shared pointer to the created or cached shader, or nullptr on
     * failure.
     */
    SharedPtr<IShader> createShaderFromFile(
      const Path& shaderPath,
      shaderStageType::Type type
    ) override;

    /**
     * @brief Creates a shader from the given string content. Returns a cached
     * instance if it already exists.
     *
     * @param shaderKey Unique key identifying the shader.
     * @param shaderCode The source code of the shader.
     * @param type The type of the shader stage.
     *
     * @return Shared pointer to the created or cached shader, or nullptr on
     * failure.
     */
    SharedPtr<IShader> createShaderFromString(
      const String& shaderKey,
      const String& shaderCode,
      shaderStageType::Type type
    ) override;

    /**
     * @brief Retrieves a cached shader by its key.
     *
     * @param shaderKey Unique key identifying the shader.
     *
     * @return Shared pointer to the cached shader, or nullptr if not found.
     */
    SharedPtr<IShader> getShader(
      const String& shaderKey
    ) const override;

    /**
     * @brief Retrieves the default vertex shader. If it does not exist, creates
     * and caches it before returning.
     *
     * @return Shared pointer to the default vertex shader.
     */
    SharedPtr<IShader> getDefaultVertexShader() override;

    /**
     * @brief Retrieves the lit vertex shader. If it does not exist, creates
     * and caches it before returning.
     *
     * @return Shared pointer to the lit vertex shader.
     */
    SharedPtr<IShader> getLitVertexShader() override;

    /**
     * @brief Retrieves the unlit fragment shader. If it does not exist, creates
     * and caches it before returning.
     *
     * @return Shared pointer to the unlit fragment shader.
     */
    SharedPtr<IShader> getUnlitFragmentShader() override;

    /**
     * @brief Retrieves the Blinn-Phong forward fragment shader. If it does not
     * exist, creates and caches it before returning.
     *
     * @return Shared pointer to the Blinn-Phong forward fragment shader.
     */
    SharedPtr<IShader> getBlinnPhongForwardFragmentShader() override;

    /**
     * @brief Clears all cached shaders from the manager.
     */
    void clear() override;

  private:
    UniquePtr<IShaderFactory> m_shaderFactory;

    void createDefaultVertexShader();
    void createLitVertexShader();
    void createUnlitFragmentShader();
    void createBlinnPhongForwardFragmentShader();
  };
}
