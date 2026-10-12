#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc::graphics::generators
{
  /**
   * @brief OpenGL implementation of the IEquirectangularToCubeMapGenerator interface.
   *
   * @note KNOWN ISSUE: For an sRGB LDR source this creates an sRGB cubemap render target,
   * but the conversion never enables GL_FRAMEBUFFER_SRGB. The shader samples linearized
   * source values and stores them unencoded; later sampling the sRGB cubemap decodes them
   * again, darkening the result. Enable and restore framebuffer-sRGB conversion while
   * rendering sRGB outputs, or render to a linear-format cubemap.
   */
  class HC_GRAPHICS_OPENGL_EXPORT OpenGlEquirectangularToCubeMapGenerator :
    public IEquirectangularToCubeMapGenerator
  {
  public:
    OpenGlEquirectangularToCubeMapGenerator(IDataBlockManager& dataBlockManager);
    ~OpenGlEquirectangularToCubeMapGenerator() override;

    /**
     * @copydoc IEquirectangularToCubeMapGenerator::generate
     */
    SharedPtr<ICubeMap> generate(
      const ITexture& equirectangularTexture,
      const CubeMapGeneratorSettings& settings
    ) override;

    /**
     * @copydoc IEquirectangularToCubeMapGenerator::destroy
     */
    void destroy() override;

    /**
     * @brief Initializes the generator.
     * @param equirectangularToCubeMapShaderProgram The shader program used for the
     * conversion.
     */
    void initialize(
      SharedPtr<IShaderProgram> equirectangularToCubeMapShaderProgram
    );

  private:
    IDataBlockManager& m_dataBlockManager;
    SharedPtr<IShaderProgram> m_equirectangularToCubemapShaderProgram;
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
