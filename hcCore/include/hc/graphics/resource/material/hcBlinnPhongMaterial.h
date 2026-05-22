#pragma once

#include "hc/graphics/resource/material/hcIMaterial.h"

namespace hc
{
  class BlinnPhongMaterialDescriptor;
  class ITexture;
  class IShaderProgram;

  /**
   * @brief Represents a material that uses the Blinn-Phong shading model for rendering.
   */
  class HC_CORE_EXPORT BlinnPhongMaterial : public IMaterial
  {
  public:
    /**
     * @brief Constructs a Blinn-Phong material with the specified material ID.
     *
     * @param materialId The unique identifier for this material, used for sorting and
     * batching.
     */
    BlinnPhongMaterial(UInt16 materialId);
    ~BlinnPhongMaterial() override;
    /**
     * @copydoc IMaterial::getId
     */
    const Id& getId() const override;
    /**
     * @copydoc IMaterial::getMaterialId
     */
    UInt16 getMaterialId() const override;
    /**
     * @copydoc IMaterial::destroy
     */
    void destroy() override;
    /**
     * @copydoc IMaterial::getShaderType
     */
    shadingType::Type getShaderType() const override;
    /**
     * @copydoc IMaterial::getRenderMode
     */
    materialRenderMode::Type getRenderMode() const override;
    /**
     * @copydoc IMaterial::isTransparent
     */
    bool isTransparent() const override;
    /**
     * @copydoc IMaterial::isAlphaCutout
     */
    bool isAlphaCutout() const override;
    /**
     * @copydoc IMaterial::bind
     */
    void bind(
      const CameraMatrices& cameraMatrices,
      renderPassType::Type renderPass
    ) override;
    /**
     * Updates the model matrix uniform in the shader program.
     *
     * @param modelMatrix The model matrix to set for this material.
     */
    void updateModelMatrix(const Matrix4& modelMatrix);
  private:
    Id m_id;
    UInt16 m_materialId;
    std::shared_ptr<BlinnPhongMaterialDescriptor> m_descriptor;
    std::shared_ptr<IShaderProgram> m_shaderProgram;
    std::shared_ptr<ITexture> m_mainTexture;
  };
}
