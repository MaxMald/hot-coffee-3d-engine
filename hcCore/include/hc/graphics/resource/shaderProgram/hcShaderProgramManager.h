#pragma once

#include "hc/graphics/resource/shaderProgram/hcIShaderProgramManager.h"

namespace hc
{
  class IShaderProgramFactory;
  class IShaderManager;

  /**
   * @brief Manages shader program resources and their creation.
   *
   * Provides functionality for creating, caching, and retrieving shader
   * programs, including custom and built-in shader programs.
   */
  class HC_CORE_EXPORT ShaderProgramManager : public IShaderProgramManager
  {
  public:
    /**
     * @brief Constructs a ShaderProgramManager with required dependencies.
     * 
     * @param shaderProgramFactory Unique pointer to the shader program factory.
     * @param shaderManager Reference to the shader manager.
     */
    ShaderProgramManager(
      UniquePtr<IShaderProgramFactory> shaderProgramFactory,
      IShaderManager& shaderManager
    );
    ~ShaderProgramManager() override;

    /**
     * @copydoc IShaderProgramManager::createShaderProgram
     */
    SharedPtr<IShaderProgram> createShaderProgram(
      const String& programKey,
      SharedPtr<IShader> vertexShader,
      SharedPtr<IShader> fragmentShader
    ) override;

    /**
     * @copydoc IShaderProgramManager::getShaderProgram
     */
    SharedPtr<IShaderProgram> getShaderProgram(
      const String& programKey
    ) const override;

    /**
     * @copydoc IShaderProgramManager::hasShaderProgram
     */
    bool hasShaderProgram(const String& programKey) const override;

    /**
     * @copydoc IShaderProgramManager::getBuiltInShaderProgram
     */
    SharedPtr<IShaderProgram> getBuiltInShaderProgram(
      builtInShaderProgramType::Type type
    ) override;

    /**
     * @copydoc IShaderProgramManager::clear
     */
    void clear() override;

  private:
    UniquePtr<IShaderProgramFactory> m_shaderProgramFactory;
    IShaderManager& m_shaderManager;
    UnorderedMap<builtInShaderProgramType::Type, SharedPtr<IShaderProgram>> m_builtInShaderPrograms;
    UnorderedMap<String, SharedPtr<IShaderProgram>> m_customShaderPrograms;

    void createBuiltInShaderProgram(
      builtInShaderProgramType::Type type,
      SharedPtr<IShader> vertexShader,
      SharedPtr<IShader> fragmentShader
    );
  };
}
