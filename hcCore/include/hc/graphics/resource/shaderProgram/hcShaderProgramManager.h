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
   * programs, including the default unlit shader program. Implements the
   * IShaderProgramManager interface.
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
     * @copydoc IShaderProgramManager::getUnlitShaderProgram
     */
    SharedPtr<IShaderProgram> getUnlitShaderProgram() override;

    /**
     * @copydoc IShaderProgramManager::getBlinnPhongForwardProgram
     */
    SharedPtr<IShaderProgram> getBlinnPhongForwardProgram() override;

    /**
     * @copydoc IShaderProgramManager::clear
     */
    void clear() override;

  private:
    UniquePtr<IShaderProgramFactory> m_shaderProgramFactory;
    IShaderManager& m_shaderManager;
    SharedPtr<IShaderProgram> m_unlitShaderProgram;
    SharedPtr<IShaderProgram> m_blinnPhongForwardProgram;

    /**
     * @brief Creates the unlit shader program and caches it.
     */
    void createUnlitShaderProgram();

    /**
     * @brief Creates the Blinn-Phong forward shader program and caches it.
     */
    void createBlinnPhongForwardProgram();
  };
}
