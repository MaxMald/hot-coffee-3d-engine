#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc::graphics::generators
{
  /**
   * @brief OpenGL implementation of the IDiffuseIrradianceCubeMapGenerator interface.
   * This class generates a diffuse irradiance cube map from an existing cube map using
   * OpenGL.
   */
  class OpenGlDiffuseIrradianceToCubeMapGenerator : public IDiffuseIrradianceCubeMapGenerator
  {
  public:
    /**
     * @brief Constructs an OpenGlDiffuseIrradianceToCubeMapGenerator with the given
     * IDataBlockManager.
     * @param dataBlockManager The IDataBlockManager used for managing data blocks.
     */
    OpenGlDiffuseIrradianceToCubeMapGenerator(IDataBlockManager& dataBlockManager);
    ~OpenGlDiffuseIrradianceToCubeMapGenerator() override;

    /**
     * @copydoc IDiffuseIrradianceCubeMapGenerator::generate
     */
    SharedPtr<ICubeMap> generate(
      const ICubeMap& inputCubeMap,
      const DiffuseIrradianceCubeMapGeneratorSettings& settings
    ) override;

    /**
     * @brief Destroys the generator and releases any associated resources.
     */
    void destroy() override;

    /**
     * @brief Initializes the generator.
     * @param diffuseIrradianceGeneratorShaderProgram The shader program used to generate
     * the diffuse irradiance cube map.
     */
    void initialize(
      SharedPtr<IShaderProgram> diffuseIrradianceGeneratorShaderProgram
    );

  private:
    IDataBlockManager& m_dataBlockManager;
    SharedPtr<IShaderProgram> m_diffuseIrradianceGeneratorShaderProgram;
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
