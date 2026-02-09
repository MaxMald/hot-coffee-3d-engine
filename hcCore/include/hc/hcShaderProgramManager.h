#pragma once

#include "hc/hcIShaderProgramManager.h"

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
  class ShaderProgramManager : public IShaderProgramManager
  {
  public:
    /**
     * @brief Constructs a ShaderProgramManager with required dependencies.
     * 
     * @param shaderProgramFactory Reference to the shader program factory.
     * @param shaderManager Reference to the shader manager.
     */
    ShaderProgramManager(
      IShaderProgramFactory& shaderProgramFactory,
      IShaderManager& shaderManager
    );
    ~ShaderProgramManager() override;

    /**
     * @brief Retrieves the default unlit shader program.
     * 
     * @return Shared pointer to the unlit shader program instance.
     */
    SharedPtr<IShaderProgram> getUnlitShaderProgram() override;

    /**
     * @brief Clears all managed shader programs and releases associated
     * resources.
     */
    void clear() override;

  private:
    IShaderProgramFactory& m_shaderProgramFactory;
    IShaderManager& m_shaderManager;
    SharedPtr<IShaderProgram> m_unlitShaderProgram;

    /**
     * @brief Creates the unlit shader program and caches it.
     */
    void createUnlitShaderProgram();
  };
}
