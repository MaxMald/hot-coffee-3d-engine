#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc::graphics::generators
{
  /**
   * @brief OpenGL implementation of the IPrefilteredEnvironmentCubeMapGenerator
   * interface.
   */
  class OpenGlPrefilteredEnvironmentCubeMapGenerator :
    public IPrefilteredEnvironmentCubeMapGenerator
  {
  public:
    OpenGlPrefilteredEnvironmentCubeMapGenerator(IDataBlockManager& dataBlockManager);
    ~OpenGlPrefilteredEnvironmentCubeMapGenerator() override;

    /**
     * @copydoc IPrefilteredEnvironmentCubeMapGenerator::generate
     */
    SharedPtr<ICubeMap> generate(
      const ICubeMap& inputCubeMap,
      const PrefilteredEnvironmentCubeMapGeneratorSettings& settings
    ) override;

    /**
     * @copydoc IPrefilteredEnvironmentCubeMapGenerator::destroy
     */
    void destroy() override;

    /**
     * @brief Initializes the OpenGL prefiltered environment cube map generator with
     * the specified shader program.
     * 
     * @param prefilteredEnvironmentGeneratorShaderProgram The shader program used for
     * generating the prefiltered environment cube map.
     */
    void initialize(
      SharedPtr<IShaderProgram> prefilteredEnvironmentGeneratorShaderProgram
    );

  private:
    IDataBlockManager& m_dataBlockManager;
    SharedPtr<IShaderProgram> m_prefilteredEnvironmentGeneratorShaderProgram;
    UInt32 m_fbo;
    UInt32 m_rbo;
    UInt32 m_currentRenderbufferSize;
    UInt32 m_boxVao;
    UInt32 m_boxVbo;
    bool m_isValid;

    /**
     * @brief Resizes the renderbuffer to the specified size.
     * @param faceSize The new size for the renderbuffer.
     */
    void resize(UInt32 faceSize);

    /**
     * @brief Creates the box geometry for rendering the cubemap faces.
     */
    void createBox();

    /**
     * @brief Creates the framebuffer object for rendering to the cubemap.
     */
    void createFramebuffer();
  };
}
