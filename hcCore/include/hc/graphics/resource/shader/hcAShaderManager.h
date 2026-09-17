#pragma once

#include "hc/graphics/resource/shader/hcIShaderManager.h"
#include "hc/graphics/resource/hcResourcesCache.h"

namespace hc
{
  /**
   * @brief Abstract base class for shader managers.
   */
  class HC_CORE_EXPORT AShaderManager : public IShaderManager
  {
  public:
    virtual ~AShaderManager() override;

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

    /**
     * @brief Destroys the shader manager and releases all associated resources.
     */
    void destroy() override;

  protected:
    /**
     * @brief Maps built-in shader types to their corresponding shader instances.
     */
    UnorderedMap<builtInShaderType::Type, SharedPtr<IShader>> m_builtInShaders;

    /**
     * @brief Maps custom shader keys to their corresponding shader instances.
     */
    UnorderedMap<String, SharedPtr<IShader>> m_customShaders;

    /**
     * @brief Procted constructor to prevent direct instantiation of the shader manager.
     */
    AShaderManager();

    /**
     * @brief Creates a concrete shader from the given shader code.
     *
     * @remark This method must be implemented by derived classes to create shaders
     * specific to the graphics API in use (e.g., OpenGL, DirectX, Vulkan).
     *
     * @param content The source code of the shader.
     * @param type The type of the shader stage.
     *
     * @return Shared pointer to the created shader.
     *
     * @throws RuntimeErrorException if the shader creation fails.
     */
    virtual SharedPtr<IShader> createConcreteShaderFromString(
      const String& content,
      shaderStageType::Type type
    ) = 0;

    /**
     * @brief Creates a concrete built-in shader of the specified type.
     *
     * @remark This method must be implemented by derived classes to create built-in
     * shaders specific to the graphics API in use (e.g., OpenGL, DirectX, Vulkan).
     *
     * @param type The type of the built-in shader to create.
     *
     * @return Shared pointer to the created built-in shader.
     *
     * @throws RuntimeErrorException if the built-in shader creation fails.
     */
    virtual SharedPtr<IShader> createConcreteBuiltInShader(
      builtInShaderType::Type type
    ) = 0;
    
    /**
     * @brief Called when the shader manager is cleared, allowing derived classes to
     * perform any necessary cleanup.
     */
    virtual void onClear() = 0;

    /**
     * @brief Called when the shader manager is destroyed, allowing derived classes to
     * perform any necessary cleanup before the manager is deleted.
     */
    virtual void onDestroy() = 0;
  };
}
